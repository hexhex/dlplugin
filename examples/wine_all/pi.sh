# $1: Instance (wine category)
# $2: Output filename

echo "
wine(X) :- &dlC[\"wine.rdf\", empty, empty, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$1\"](X).

% 1. guess the extension
in_not_dry(X) :- wine(X), not out_not_dry(X).
out_not_dry(X) :- wine(X), not in_not_dry(X).

% 2. check compliance of the guess with the ontology
mcdry(\"DryWine\", X) :- in_not_dry(X).
:- &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"-http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>, out_not_dry(X).

% 3. apply the default
mcdry2(\"DryWine\", X) :- p_not_dry(X).
p_not_dry(X) :- wine(X), &dlC[\"wine.rdf\", empty, mcdry2, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$1\"](X), not &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>.

% 4. check compliance of the extension
:- not &dlC[\"wine.rdf\", empty, mcdry2, empty, empty, \"-http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>, in_not_dry(X).
:- &dlC[\"wine.rdf\", empty, mcdry2, empty, empty, \"-http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>, out_not_dry(X).
" > $2
