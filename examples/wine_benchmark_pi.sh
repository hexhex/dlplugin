alarm() { perl -e 'alarm shift; exec @ARGV' "$@"; }

if [ $# -le 0 ]; then
	echo "Usage:"
	echo "     benchmark.sh [Timeout/s] [Configuration Strings (optional)]"
	echo "The configuration string consists of dlvhex options with semicolon as delimiter"
	exit 0
fi

# $2: configurations
if [ $# -le 1 ]; then
	confstr="--solver=genuinegc;--solver=genuinegc --extlearn"
else
	confstr=$2
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


# create working directory
wd=`mktemp -d $PWD/tmp.XXXXXXXXXX`


# $1: program
# $2: min domain size
# $3: max domain size
# $4: timeout in seconds

categories=(AlsatianWine AmericanWine Beaujolais Bordeaux Burgundy CabernetFranc CabernetSauvignon CaliforniaWine Chardonnay CheninBlanc DessertWine DryWine EarlyHarvest FrenchWine FullBodiedWine Gamay GermanWine ItalianWine LateHarvest Loire Meritage Merlot PetiteSyrah PinotBlanc PinorNoir RedWine Riesling RoseWine SemillonOrSauvignonBlanc SweetWine TableWine TexasWine WhiteWine Zinfandel)

for category in ${categories[@]}
do

	echo "
	wine(X) :- &dlC[\"wine.rdf\", empty, empty, empty, empty, \"$category\"](X).

	% 1. guess the extension
	in_not_dry(X) :- wine(X), not out_not_dry(X).
	out_not_dry(X) :- wine(X), not in_not_dry(X).

	% 2. check compliance of the guess with the ontology
	mcdry(\"DryWine\", X) :- in_not_dry(X).
	:- &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"-DryWine\"](X)<fullylinear>, out_not_dry(X).

	% 3. apply the default
	mcdry2(\"DryWine\", X) :- p_not_dry(X).
	p_not_dry(X) :- wine(X), &dlC[\"wine.rdf\", empty, mcdry2, empty, empty, \"$category\"](X), not &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"DryWine\"](X)<fullylinear>.

	% 4. check compliance of the extension
	:- not &dlC[\"wine.rdf\", empty, mcdry2, empty, empty, \"-DryWine\"](X)<fullylinear>, in_not_dry(X).
	:- &dlC[\"wine.rdf\", empty, mcdry2, empty, empty, \"-DryWine\"](X)<fullylinear>, out_not_dry(X).

" > $wd/prog.hex

	line="$category"
	i=0
	for c in "${confs[@]}"
	do
		alarm $1 /usr/bin/time -o time.txt -f %e dlvhex2 $c $wd/prog.hex 2>/dev/null >/dev/null
		output=`cat time.txt`
		if [ "$output" = "" ]; then
			output="TO"
		fi
		line="$line   $output"

		let i=i+1
	done
	echo $line
done

# cleanup
rm -r $wd
