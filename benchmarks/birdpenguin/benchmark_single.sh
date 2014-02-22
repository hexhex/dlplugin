# default parameters
confstr="--flpcheck=explicit --noflpcriterion;--flpcheck=explicit --extlearn --noflpcriterion;--flpcheck=ufsm --noflpcriterion;--flpcheck=ufsm --extlearn --noflpcriterion;--flpcheck=ufsm --extlearn --ufslearn --noflpcriterion;--flpcheck=ufs;--flpcheck=ufs --extlearn;--flpcheck=ufs --extlearn --ufslearn;--flpcheck=aufs;--flpcheck=aufs --extlearn;--flpcheck=aufs --extlearn --ufslearn;--flpcheck=explicit --noflpcriterion -n=1;--flpcheck=explicit --extlearn --noflpcriterion -n=1;--flpcheck=ufsm -n=1;--flpcheck=ufsm --extlearn --noflpcriterion -n=1;--flpcheck=ufsm --extlearn --ufslearn --noflpcriterion -n=1;--flpcheck=ufs -n=1;--flpcheck=ufs --extlearn -n=1;--flpcheck=ufs --extlearn --ufslearn -n=1;--flpcheck=aufs -n=1;--flpcheck=aufs --extlearn -n=1;--flpcheck=aufs --extlearn --ufslearn -n=1"
confstr2=$(cat conf)
if [ $? == 0 ]; then
        confstr=$confstr2
fi

export PATH=$1
export LD_LIBRARY_PATH=$2
maxize=$3
to=$4
maxsize=20

# split configurations
IFS=';' read -ra confs <<< "$confstr"
header="#size"
i=0
for c in "${confs[@]}"
do
	timeout[$i]=0
	header="$header   \"$c\""
	let i=i+1
done
echo $header

# for all domain sizes
for (( size = 1; size <= $maxsize; size++ ))
do
	echo -ne "$size"

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
		birds(X) :- &dlC[\"b.owl\", pcbird, mcbird, prbird, mrbird, \"Bird\"](X).
		flies(X) :- birds(X), not neg_flies(X).

		% Single out the non-fliers under default assumption:
		pcflier(\"Flier\", X) :- flies(X).
		neg_flies(X) :- birds(X), &dlC[\"b.owl\", pcflier, mcflier, prflier, mrflier, \"-Flier\"](X)<fullylinear>.

		% Is the description logic KB inconsistent? 
		inconsistent :- not &dlConsistent[\"b.owl\", pcflier, mcflier, prflier, mrflier]()." > prog.hex

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
		  </owl:Class>" > b.owl
	echo $domain >> b.owl
	echo "</rdf:RDF>" >> b.owl

	# for all configurations
	i=0
	for c in "${confs[@]}"
	do
		echo -ne -e " "
			# run racer
			pkill "RacerPro"
			RacerPro >/dev/null &
			rpid=$!

			# run dlvhex
			$(timeout $to time --quiet -o time.dat -f %e dlvhex2 --verbose=0 $c --plugindir=../../src/ prog.hex 2>/dev/null >/dev/null)
			ret=$?
			output=$(cat time.dat)
			if [[ $ret != 0 ]]; then
				output="xxx"
			fi
			if [[ $ret == 124 ]]; then
				output="---"
			fi

			# kill racer
			kill $rpid
			wait $rpid >/dev/null 2>/dev/null

			rm time.dat
		echo -ne $output
		let i=i+1
	done
	echo -e -ne "\n"
done

rm prog.hex
rm b.owl
