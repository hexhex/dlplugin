#!/bin/bash

runheader=$(which dlvhex_run_header.sh)
if [[ $runheader == "" ]] || [ $(cat $runheader | grep "dlvhex_run_header.sh Version 1." | wc -l) == 0 ]; then
	echo "Could not find dlvhex_run_header.sh (version 1.x); make sure that the benchmarks/script directory is in your PATH"
	exit 1
fi
source $runheader

# run instances
if [[ $all -eq 1 ]]; then
	# run all instances using the benchmark script run insts
	$bmscripts/runinsts.sh "wine" "$mydir/run.sh" "$mydir" "$to" "" "" "$req"
else

	confstr="--flpcheck=explicit;--flpcheck=explicit --extlearn;--flpcheck=ufsm --noflpcriterion;--flpcheck=ufsm --extlearn --noflpcriterion;--flpcheck=ufsm --extlearn --ufslearn --noflpcriterion;--flpcheck=ufs;--flpcheck=ufs --extlearn;--flpcheck=ufs --extlearn --ufslearn;--flpcheck=aufs;--flpcheck=aufs --extlearn;--flpcheck=aufs --extlearn --ufslearn;--flpcheck=explicit -n=1;--flpcheck=explicit --extlearn -n=1;--flpcheck=ufsm -n=1;--flpcheck=ufsm --extlearn --noflpcriterion -n=1;--flpcheck=ufsm --extlearn --ufslearn --noflpcriterion -n=1;--flpcheck=ufs -n=1;--flpcheck=ufs --extlearn -n=1;--flpcheck=ufs --extlearn --ufslearn -n=1;--flpcheck=aufs -n=1;--flpcheck=aufs --extlearn -n=1;--flpcheck=aufs --extlearn --ufslearn -n=1"

	# prepare category instances
	categories=(AlsatianWine AmericanWine Beaujolais Bordeaux Burgundy CabernetFranc CabernetSauvignon CaliforniaWine Chardonnay CheninBlanc DessertWine DryWine EarlyHarvest FrenchWine FullBodiedWine Gamay GermanWine ItalianWine LateHarvest Loire Meritage Merlot PetiteSyrah PinotBlanc PinorNoir RedWine Riesling RoseWine SemillonOrSauvignonBlanc SweetWine TableWine TexasWine WhiteWine Zinfandel)

	# split configurations
	IFS=';' read -ra confs <<< "$confstr;"
	header="#instance"
	i=0
	for c in "${confs[@]}"
	do
		header="$header   \"$c\""
		let i=i+1
	done
	echo $header

	# run single instance
	# Note: Since Racer cannot be run in parallel, we consider the set of all instances sizes as "one instance"
	for ontology in ontologies/*.owl
	do
		# for all wine categories
		for category in ${categories[@]}
		do

			command="dlvhex2 --plugindir=../../src CONF prog.hex"

			# do benchmark

			echo -ne "wine_${ontology}_$category 1"	# 1 because we want to count instances

			# for all configurations
			timefile=$(mktemp)
			stdoutfile=$(mktemp)
			stderrfile=$(mktemp)
			i=0

			# write HEX program
			echo "
			wine(X) :- &dlC[\"$ontology\", empty, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$category\"](X).

			% By default, a wine is white:
			whitewine(\"WhiteWine\", X) :- wine(X), not redwine(X).

			% Single out the red wines under default assumption:
			redwine(X) :- wine(X), &dlC[\"$ontology\", whitewine, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#RedWine\"](X)<fullylinear>.

			inconsistent :- not &dlConsistent[\"$ontology\", whitewine, empty, empty, empty]().
			:- inconsistent.
			" > prog.hex

			for c in "${confs[@]}"
			do
				echo -ne -e " "

				# prepare command
				fullcommand=${command/CONF/$c}
				fullcommand=${fullcommand/INST/$instance}
				cmd="timeout $to time -o $timefile -f %e $fullcommand"

				# run racer
				racerpath=$(which RacerPro)
				if [[ $racerpath == "" ]]; then
					echo "RacerPro could not be found"
					exit 1
				fi
				$racerpath >/dev/null &
				rpid=$!

				# execute
				eval "$cmd >$stdoutfile 2>$stderrfile"
				ret=$?

				# build output
				output=$($bmscripts/timeoutputbuilder.sh $ret $timefile $stdoutfile $stderrfile)
				obresult=$?
				if [ $obresult -eq 0 ]; then
					echo -ne "$output"
				elif [ $obresult -eq 2 ]; then
					echo "Error during execution of: \"$fullcommand\"" >&2
					echo ">> Stdout:" >&2
					cat $stdoutfile >&2
					echo ">> Stderr:" >&2
					cat $stderrfile >&2
					echo -ne "$output"
				else
					echo "Output builder for command \"$fullcommand\" failed" >&2
					# kill racer and exit
					pkill $rpid
					exit 1
				fi

				# kill racer
				pkill -9 $rpid

				let i=i+1
			done
			echo -e -ne "\n"
			rm prog.hex
			rm $timefile
			rm $stdoutfile
			rm $stderrfile
		done
	done
fi

