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

		% By default, a wine is white:
		whitewine(\"WhiteWine\", X) :- wine(X), not redwine(X).

		% Single out the red wines under default assumption:
		redwine(X) :- wine(X), &dlC[\"wine.rdf\", whitewine, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#RedWine\"](X)<fullylinear>.

		inconsistent :- not &dlConsistent[\"wine.rdf\", whitewine, empty, empty, empty]().
		:- inconsistent.
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

			# kill dlvhex and dlv
			pkill -9 -u $USER dlvhex2
			pkill -9 -u $USER dlv


			echo -ne $output
			let i=i+1
		done
		echo ""
	done
done

# cleanup
rm wine.rdf
rm prog.hex
