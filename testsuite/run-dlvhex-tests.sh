#!/bin/bash

DLVHEX=dlvhex

TMPFILE=$(mktemp)

cd $EXAMPLES

failed=0
ntests=0

echo ============ dlvhex tests start ============

for t in $(find -name '*.test' -type f)
do
    while read HEXPROGRAM ANSWERSETS
    do
	let ntests++

	$DLVHEX -s $HEXPROGRAM | egrep -v "^$" > $TMPFILE
	
	if cmp -s $TMPFILE $ANSWERSETS
	then
	    echo PASS: $HEXPROGRAM
	else
	    echo FAIL: $HEXPROGRAM 

	    # and now check which answersets differ

	    pasted=$(mktemp)
	    paste $ANSWERSETS $TMPFILE > $pasted

	    from=$(mktemp)
	    to=$(mktemp)

	    OLDIFS=$IFS
	    IFS=" " # we need the tabs for cut

	    nas=1

	    while read
	    do
		echo $REPLY | cut -f1 > $from
		echo $REPLY | cut -f2 > $to
		if ! cmp -s $from $to
		then
		    echo    Answerset $nas differs
		fi
		let nas++
	    done < $pasted # redirected pasted file to the while loop

	    IFS=$OLDIFS

	    rm -f $pasted $from $to

	    let failed++
	fi
    done < $t # redirect test file to the while loop
done

# cleanup
rm -f $TMPFILE

echo ========== dlvhex tests completed ==========

echo Tested $ntests dlvhex programs
echo Failed tests: $failed

echo ============= dlvhex tests end =============

exit $failed