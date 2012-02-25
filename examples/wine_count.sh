# create working directory
wd=`mktemp -d $PWD/tmp.XXXXXXXXXX`


categories=(AlsatianWine AmericanWine Beaujolais Bordeaux Burgundy CabernetFranc CabernetSauvignon CaliforniaWine Chardonnay CheninBlanc DessertWine DryWine EarlyHarvest FrenchWine FullBodiedWine Gamay GermanWine ItalianWine LateHarvest Loire Meritage Merlot PetiteSyrah PinotBlanc PinorNoir RedWine Riesling RoseWine SemillonOrSauvignonBlanc SweetWine TableWine TexasWine WhiteWine Zinfandel)

for category in ${categories[@]}
do

	echo "
	#maxint=1000.

	wine(X) :- &dlC[\"wine.rdf\", pcwine, mcwine, prwine, mrwine, \"$category\"](X).
	cnt(\"$category\", C) :- C = #count{ X : wine(X) }.

" > $wd/prog.hex

	dlvhex2 --silent $wd/prog.hex
done

# cleanup
rm -r $wd
