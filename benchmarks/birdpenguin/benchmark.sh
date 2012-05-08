# default parameters
if [ $# -le 0 ]; then
	confstr="--solver=dlv --flpcheck=none;--solver=genuinegc --flpcheck=none;--solver=genuinegc --extlearn --flpcheck=none"
else
	confstr=$1
fi
if [ $# -le 1 ]; then
	to=300
else
	to=$2
fi
if [ $# -le 2 ]; then
	maxsize=20
else
	maxsize=$3
fi

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

# run racer
RacerPro >/dev/null &
rpid=$!

# for all domain sizes
for (( size = 1; size <= $maxsize; size++ ))
do
	echo -ne "$size:"

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


		% By default, a bird does not fly:
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
		# if a configuration timed out, then it can be skipped for larger instances
		if [ ${timeout[$i]} -eq 0 ]; then

			# run dlvhex
			output=$(timeout $to time -f %e dlvhex2 $c --plugindir=../../src/ prog.hex 2>&1 >/dev/null)
			if [[ $? == 124 ]]; then
				output="---"
				timeout[$i]=1
			fi
		else
			output="---"
		fi
		echo -ne $output
		let i=i+1
	done
	echo -e -ne "\n"
done

# kill racer
pkill $rpid

rm prog.hex
rm b.owl
