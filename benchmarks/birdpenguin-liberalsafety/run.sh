#!/bin/bash

source dlvhex_run_header.sh

# run instances
if [[ $all -eq 1 ]]; then
	# run all instances using the benchmark script run insts
	$bmscripts/runinsts.sh "20" "$mydir/run.sh" "$mydir" "$to"	# Note: Here the condition "20" defines the maximum size
else

	confstr="--extlearn --flpcheck=aufs prog.hex;--extlearn --flpcheck=aufs --liberalsafety prognd.hex"

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
		command="dlvhex2 --plugindir=../../src CONF"

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
			inconsistent :- not &dlConsistent[\"b$size.owl\", pcflier, mcflier, prflier, mrflier]()." > prog.hex

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
		        birds(X) :- &dlC[\"b.owl\", pcbird, mcbird, prbird, mrbird, \"Bird\"](X).
		        flies(X) :- birds(X), not neg_flies(X).

		        % Single out the non-fliers under default assumption:
		        pcflier(\"Flier\", X) :- flies(X).
		        neg_flies(X) :- birds(X), &dlC[\"b.owl\", pcflier, mcflier, prflier, mrflier, \"-Flier\"](X)<fullylinear>.

		        % Is the description logic KB inconsistent?
		        inconsistent :- not &dlConsistent[\"b.owl\", pcflier, mcflier, prflier, mrflier]()." > prognd.hex

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
			output=$($bmscripts/gstimeoutputbuilder.sh $ret $timefile $stdoutfile $stderrfile)
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
		rm prog.hex
		rm prognd.hex
		rm b$size.owl
		rm $timefile
		rm $stdoutfile
		rm $stderrfile
	done
fi

