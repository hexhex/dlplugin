#!/bin/bash

MKTEMP="mktemp -t tmp.XXXXXXXXXX"
TMPFILE=$($MKTEMP) # global temp. file for answer sets

failed=0
warned=0
ntests=0

cd $EXAMPLES

echo ============ dlvhex tests start ============

for t in $(find -name '*.test' -type f)
do
    while read HEXPROGRAM ANSWERSETS ADDPARM
    do
	let ntests++

	#HEXPROGRAM=$TESTDIR/$HEXPROGRAM
	#ANSWERSETS=$TESTDIR/$ANSWERSETS

	if [ ! -f $HEXPROGRAM ] || [ ! -f $ANSWERSETS ]; then
	    test ! -f $HEXPROGRAM && echo WARN: Could not find program file $HEXPROGRAM
	    test ! -f $ANSWERSETS && echo WARN: Could not find answer sets file $ANSWERSETS
	    continue
	fi

	# run dlvhex with specified parameters and program
	$DLVHEX  $PARAMETERS $ADDPARM $HEXPROGRAM | egrep -v "^$" > $TMPFILE

	if cmp -s $TMPFILE $ANSWERSETS
	then
	    echo PASS: $HEXPROGRAM
	else
	    # and now check which answersets differ

	    pasted=$($MKTEMP)
	    paste $ANSWERSETS $TMPFILE > $pasted

	    OLDIFS=$IFS
	    IFS=" " # we need the tabs for cut

	    nas=1 # counter for answer sets

            # todo: handle different costs in case of weak constraints!

	    while read
	    do
		# translate both answersets to python lists
		a1=$(echo $REPLY | cut -f1 | sed s/"'"/"\\\'"/g | sed s/"{"/"['"/ | sed s/", "/"', '"/g | sed s/"}"/"']"/)
		a2=$(echo $REPLY | cut -f2 | sed s/"'"/"\\\'"/g | sed s/"{"/"['"/ | sed s/", "/"', '"/g | sed s/"}"/"']"/)


		# now check if set difference yields incomparability
		if cat <<EOF | python
# -*- coding: utf-8 -*-
import sys
a1 = $a1
a2 = $a2
z1 = zip(a1,a2)
z2 = zip(z1, range(len(z1)))
z3 = [ e for e in z2 if e[0][0] != e[0][1] ]
for e in z3: print 'In Answerset ' + str($nas) + ' (fact ' + str(e[1]) + '): ' + e[0][0] + ' vs. ' + e[0][1]
s1 = set(a1)
s2 = set(a2)
sys.exit(len(s1.symmetric_difference(s2)))
EOF
		then
		    echo "WARN: $HEXPROGRAM (answerset $nas has different ordering)"
		    let warned++
		else
		    echo "FAIL: $HEXPROGRAM (answerset $nas differs)"
		    let failed++
		fi

		let nas++
	    done < $pasted # redirected pasted file to the while loop

	    IFS=$OLDIFS

	    rm -f $pasted
	fi
    done < $t # redirect test file to the while loop
done

# cleanup
rm -f $TMPFILE

echo ========== dlvhex tests completed ==========

echo Tested $ntests dlvhex programs
echo $failed failed tests, $warned warnings

echo ============= dlvhex tests end =============

exit $failed
