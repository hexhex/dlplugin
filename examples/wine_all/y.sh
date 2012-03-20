# $1: Instance (wine category)
# $2: Output filename

echo "
wine(X) :- &dlC[\"wine.rdf\", empty, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$1\"](X)<fullylinear>.

cons_not_dry(X) :- wine(X), not out_cons_not_dry(X).
out_cons_not_dry(X) :- wine(X), not cons_not_dry(X).

mcdry(\"DryWine\", X) :- in_not_dry(X).
in_not_dry(X) :- wine(X), &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$1\"](X)<fullylinear>, cons_not_dry(X).

:- wine(X), &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>, cons_not_dry(X).
:- wine(X), not &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>, out_cons_not_dry(X).
" > $2
