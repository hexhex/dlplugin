# $1: Instance (wine category)
# $2: Output filename

echo "
% By default, a wine is not dry:
wine(X) :- &dlC[\"wine.rdf\", pcwine, mcwine, prwine, mrwine, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#$1\"](X).
notdry(X) :- wine(X), not dry(X).

% Single out the dry wines under default assumption:
mcdry(\"DryWine\", X) :- notdry(X).
dry(X) :- wine(X), &dlC[\"wine.rdf\", empty, mcdry, empty, empty, \"http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#DryWine\"](X)<fullylinear>.

inconsistent :- not &dlConsistent[\"wine.rdf\", empty, mcdry, empty, empty]().
:- inconsistent.
" > $2
