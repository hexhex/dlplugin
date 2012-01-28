if [ $# -le 3 ]; then
	echo "Usage:"
	echo "     benchmark.sh [Program] [Min Domain Size] [Max Domain Size] [Timeout/s] [Configuration Strings (optional)]"
	echo "The configuration string consists of dlvhex options with semicolon as delimiter"
	exit 0
fi

# $5: configurations
if [ $# -le 4 ]; then
	confstr=";--internalsolver;--internalsolver --extlearn"
else
	confstr=$5
fi
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


# $1: program
# $2: min domain size
# $3: max domain size
# $4: timeout in seconds

for (( size = $2 ; size <= $3 ; size++ ))
do

	# construct owl file
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

	# construct program
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

	line="$size"
	i=0
	for c in "${confs[@]}"
	do
		if [ ${timeout[$i]} -eq 0 ]; then
			output=`/usr/bin/time -f %e dlvhex $c $1 2>&1 >/dev/null`
			timeout[$i]=`echo "$output > $4" | bc`
			if [ ${timeout[$i]} -eq 1 ]; then
				output=$4
			fi
			line="$line   $output"
		else
			line="$line   $4"
		fi
		let i=i+1
	done
	echo $line
done
