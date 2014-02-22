# check input validity
inputok=1
if [[ $# -eq 0 ]]; then
	inputok=1
elif [[ $# -gt 5 ]]; then
	inputok=0
elif [[ $1 != "all" ]] && [[ $1 != "single" ]]; then
	inputok=0
fi
if [[ $inputok -eq 0 ]]; then
	echo "This script expects 0 to 5 parameters" >&2
	echo "   \$1: (optional) \"all\" or \"single\", default is \"all\"" >&2
	echo "   (a) If \$1=\"all\" then there are no further parameters" >&2
	echo "       \$2: (optional) timeout, default is 300" >&2
	echo "       \$3: (optional) directory with the benchmark scripts" >&2
	echo "   (b) If \$1=\"single\" then" >&2
	echo "       \$2: instance name" >&2
	echo "       \$3: timeout in seconds" >&2
	echo "       \$4: directory with the benchmark scripts" >&2
	echo "       \$5: (optional) timeout, default is 300" >&2
	exit 1
fi

# set default values
# and get location of benchmark scripts
if [[ $# -eq 0 ]]; then
	all=1
elif [[ $1 == "all" ]]; then
	all=1
else
	all=0
fi
if [[ $all -eq 1 ]]; then
	if [[ $# -ge 2 ]]; then
		to=$2
	else
		to=300
	fi
	if [[ $# -ge 3 ]]; then
		bmscripts=$3
	else
		runinstsdir=$(which runinsts.sh | head -n 1)
		if [ -e "$runinstsdir" ]; then
			bmscripts=$(dirname "$runinstsdir")
		fi
	fi
else
	instance=$2
	to=$3
	bmscripts=$4
fi
if ! [ -e "$bmscripts" ]; then
	echo "Could not find benchmark scripts"
	exit 1
fi

# run instances
mydir="$(dirname $0)"
mydir=$(cd $mydir; pwd)
if [[ $all -eq 1 ]]; then
	# run all instances using the benchmark script run insts
	$bmscripts/runinsts.sh "20" "$mydir/run.sh" "$mydir" "$to"	# Note: Here the condition "20" defines the maximum size
else

	confstr="--flpcheck=explicit;--flpcheck=explicit --extlearn;--flpcheck=ufsm --noflpcriterion;--flpcheck=ufsm --extlearn --noflpcriterion;--flpcheck=ufsm --extlearn --ufslearn --noflpcriterion;--flpcheck=ufs;--flpcheck=ufs --extlearn;--flpcheck=ufs --extlearn --ufslearn;--flpcheck=aufs;--flpcheck=aufs --extlearn;--flpcheck=aufs --extlearn --ufslearn;--flpcheck=explicit -n=1;--flpcheck=explicit --extlearn -n=1;--flpcheck=ufsm -n=1;--flpcheck=ufsm --extlearn --noflpcriterion -n=1;--flpcheck=ufsm --extlearn --ufslearn --noflpcriterion -n=1;--flpcheck=ufs -n=1;--flpcheck=ufs --extlearn -n=1;--flpcheck=ufs --extlearn --ufslearn -n=1;--flpcheck=aufs -n=1;--flpcheck=aufs --extlearn -n=1;--flpcheck=aufs --extlearn --ufslearn -n=1"

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
	for ((size=1; $size <= $instance; size++))
	do
		command="dlvhex2 --plugindir=../../src CONF prog$size.hex"

		# do benchmark
		echo -ne "$size 1"	# 1 because we want to count instances

		# write HEX program
		echo "
			%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			%
			% Tweety (1) -- The Bird Case     
			%
			% This is the formulation of the famous \"birds fly by default\" 
			% example from the non-monotonic reasoning literature where 
			% Tweety is known to be a bird.
			%
			% The OWL ontology contains the knowledge about Birds,
			% Penguins and Fliers, and that Tweety is a bird; the birds-fly-
			% by-default rule is formulated on top of the ontology by
			% nonmonotonic rules.
			% 
			% We then can query whether Tweety flies, and get the intuitive 
			% result.
			%
			% We don't use here strong negation (\"-\") on LP predicates in rules, 
			% since well-founded semantics for dl-programs is only defined-
			% in absence of "-". As for answer set semantics, just replace
			% \"neg_\" by \"-\".
			%
			%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


			% By default, a bird flies:
			birds(X) :- &dlC[\"b$size.owl\", pcbird, mcbird, prbird, mrbird, \"Bird\"](X).
			flies(X) :- birds(X), not neg_flies(X).

			% Single out the non-fliers under default assumption:
			pcflier(\"Flier\", X) :- flies(X).
			neg_flies(X) :- birds(X), &dlC[\"b$size.owl\", pcflier, mcflier, prflier, mrflier, \"-Flier\"](X)<fullylinear>.

			% Is the description logic KB inconsistent? 
			inconsistent :- not &dlConsistent[\"b$size.owl\", pcflier, mcflier, prflier, mrflier]()." > prog$size.hex

		# write ontology
		domain=""
		for (( i = 1 ; i <= $size ; i++ ))
		do
			rem=$(( $i % 2 ))
			if [ $rem -eq 0 ]; then
				domain="$domain <Bird rdf:ID=\"Individum$i\"/>"
			else
				domain="$domain <Penguin rdf:ID=\"Individum$i\"/>"
			fi
		done
		echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>
			<!DOCTYPE rdf:RDF [] >
			<rdf:RDF
			  xmlns:owl=\"http://www.w3.org/2002/07/owl#\"
			  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"
			  xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"
			  xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"
			  xmlns=\"http://www.kr.tuwien.ac.at/staff/roman/tweety_bird#\"
			  xml:base=\"http://www.kr.tuwien.ac.at/staff/roman/tweety_bird\">

			  <owl:Ontology rdf:ID=\"tweety_bird\"/>

			  <owl:Class rdf:ID=\"Bird\" />
			  <owl:Class rdf:ID=\"Flier\" />
			  <owl:Class rdf:ID=\"NonFlier\">
			    <owl:complementOf rdf:resource=\"#Flier\" />
			  </owl:Class>

			  <owl:Class rdf:ID=\"Penguin\">
			    <rdfs:subClassOf rdf:resource=\"#Bird\" />
			    <rdfs:subClassOf rdf:resource=\"#NonFlier\" />
			  </owl:Class>" > b$size.owl
		echo $domain >> b$size.owl
		echo "</rdf:RDF>" >> b$size.owl

		# for all configurations
		timefile=$(mktemp)
		stdoutfile=$(mktemp)
		stderrfile=$(mktemp)
		i=0

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
			$racerpath/RacerPro >/dev/null &
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
			pkill $rpid

			let i=i+1
		done
		echo -e -ne "\n"
		rm prog$size.hex
		rm b$size.owl
		rm $timefile
		rm $stdoutfile
		rm $stderrfile
	done
fi

