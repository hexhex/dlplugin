#!/bin/bash

RACER=RacerPro

PID=`pidof $RACER`

if [ -z "$PID" ]
then

    $RACER -http 0 >/dev/null 2>&1 &

else

    kill $PID

fi

exit 0