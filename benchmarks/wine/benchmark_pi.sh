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

# for all ontologies
for instance in ontologies/*.owl
do
	echo "Instance: $instance"

	# write ontology
	cp $instance wine.rdf

	# prepare category instances
	categories=(AlsatianWine AmericanWine Beaujolais Bordeaux Burgundy CabernetFranc CabernetSauvignon CaliforniaWine Chardonnay CheninBlanc DessertWine DryWine EarlyHarvest FrenchWine FullBodiedWine Gamay GermanWine ItalianWine LateHarvest Loire Meritage Merlot PetiteSyrah PinotBlanc PinorNoir RedWine Riesling RoseWine SemillonOrSauvignonBlanc SweetWine TableWine TexasWine WhiteWine Zinfandel)

	# for all wine categories
	for category in ${categories[@]}
	do

		echo -ne "$category: "

		# write HEX program
		echo "
		wine(X) :- &dlC[\"wine.rdf\", empty, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$category\"](X).

		% 1. guess the extension
		in_whitewine(X) :- wine(X), not out_whitewine(X).
		out_whitewine(X) :- wine(X), not in_whitewine(X).

		% 2. check compliance of the guess with the ontology
		whitewine(\"WhiteWine\", X) :- in_whitewine(X).
		:- &dlC[\"wine.rdf\", whitewine, empty, empty, empty, \"-http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#RedWine\"](X)<fullylinear>, out_whitewine(X).

		% 3. apply the default
		whitewine2(\"WhiteWine\", X) :- p_whitewine(X).
		p_whitewine(X) :- wine(X), &dlC[\"wine.rdf\", whitewine2, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$category\"](X), not &dlC[\"wine.rdf\", whitewine, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#RedWine\"](X)<fullylinear>.

		% 4. check compliance of the extension
		:- not &dlC[\"wine.rdf\", whitewine2, empty, empty, empty, \"-http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#RedWine\"](X)<fullylinear>, in_whitewine(X).
		:- &dlC[\"wine.rdf\", whitewine2, empty, empty, empty, \"-http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#RedWine\"](X)<fullylinear>, out_whitewine(X).
		" > prog.hex

		for c in "${confs[@]}"
		do
			echo -ne -e " "
			# run racer
			RacerPro >/dev/null &
			rpid=$!

			# run dlvhex
			output=$(timeout $to time -f %e dlvhex2 $c --plugindir=../../src/ prog.hex 2>&1 >/dev/null)
			if [[ $? == 124 ]]; then
				output="---"
				timeout[$i]=1
			fi

			# kill racer
			pkill $rpid

			echo -ne $output
			let i=i+1
		done
		echo ""
	done
done

# cleanup
rm wine.rdf
rm prog.hex
