# create working directory
wd=`mktemp -d $PWD/tmp.XXXXXXXXXX`


categories=(AlsatianWine AmericanWine Beaujolais Bordeaux Burgundy CabernetFranc CabernetSauvignon CaliforniaWine Chardonnay CheninBlanc DessertWine DryWine EarlyHarvest FrenchWine FullBodiedWine Gamay GermanWine ItalianWine LateHarvest Loire Meritage Merlot PetiteSyrah PinotBlanc PinorNoir RedWine Riesling RoseWine SemillonOrSauvignonBlanc SweetWine TableWine TexasWine WhiteWine Zinfandel)

for category in ${categories[@]}
do

	echo "
	#maxint=1000.

	wine(X) :- &dlC[\"wine.rdf\", pcwine, mcwine, prwine, mrwine, \"$category\"](X).
	dry(X) :- &dlC[\"wine.rdf\", pcwine, mcwine, prwine, mrwine, \"DryWine\"](X), wine(X).
	cnt(\"$category\", CW, CD) :- CW = #count{ W : wine(W) }, CD = #count{ D : dry(D) }.

" > $wd/prog.hex

	dlvhex2 --silent --plugindir=../src/.libs/ $wd/prog.hex
done

# cleanup
rm -r $wd
