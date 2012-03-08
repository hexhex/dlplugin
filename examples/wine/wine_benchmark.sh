alarm() { perl -e 'alarm shift; exec @ARGV' "$@"; }

# $1: Name of script to write hex program
# $2: Timeout in s
# $3: Number of runs

writeProgScript=$1
timeout=$2
cntruns=$3

# set pathes
export PATH=$PATH:/mnt/lion/home/redl/local/bin/:/mnt/lion/home/redl/RacerPro-1-9-0-Linux32-ACL/
export LD_LIBRARY_PATH=$PATH:/mnt/lion/home/redl/local/lib/
##export PATH=$PATH:$HOME/RacerPro-1-9-0-Linux32-ACL/

# create working directory
wd=`mktemp -d $PWD/tmp.XXXXXXXXXX`

# configurations: without and with external learning
confstr="--solver=genuinegc --plugindir=../../src/.libs/;--solver=genuinegc --extlearn --plugindir=../../src/.libs/"
IFS=';' read -ra confs <<< "$confstr"
header="#instance"
for (( r = 0; r < $cntruns; r++ ))
do
	i=0
	for c in "${confs[@]}"
	do
		header="$header   $r.c$i"
		let i=i+1
	done
done
echo $header

# prepare instances
categories=(AlsatianWine AmericanWine Beaujolais Bordeaux Burgundy CabernetFranc CabernetSauvignon CaliforniaWine Chardonnay CheninBlanc DessertWine DryWine EarlyHarvest FrenchWine FullBodiedWine Gamay GermanWine ItalianWine LateHarvest Loire Meritage Merlot PetiteSyrah PinotBlanc PinorNoir RedWine Riesling RoseWine SemillonOrSauvignonBlanc SweetWine TableWine TexasWine WhiteWine Zinfandel)

# for each instance
for category in ${categories[@]}
do

	# write program
	./$writeProgScript $category $wd/prog.hex

	line="$category"

	# we make 5 runs
	for (( run = 0; run < $cntruns; run++ ))
	do
		# for each configuration
		for c in "${confs[@]}"
		do
			# run racer
			RacerPro >/dev/null 2>/dev/null &

			# execute dlvhex
			alarm $timeout /usr/bin/time -o time.txt -f %e dlvhex2 $c $wd/prog.hex 2>/dev/null >/dev/null

			output=`cat time.txt`
			if [ "$output" = "" ]; then
				output="TO"
			fi
			line="$line   $output"

			# stop racer
			pkill -9 RacerPro
		done
	done
	echo $line
#exit
done

# cleanup
rm -r $wd
