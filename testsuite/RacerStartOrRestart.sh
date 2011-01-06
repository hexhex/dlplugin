#!/bin/bash

RACER=RacerPro
RACERSTARTSCRIPT=RacerPro.sh

# make sure to kill it, especially if it is hanging
killall -9 $RACER

# start racer in a screen called 'racer' in detached mode
# connect to it via "screen -x racer"
#screen -S racer -d -m $RACERSTARTSCRIPT
# screen is not good if user has no homedir and no password and is primarily used via sudo

#start it in background and discard all output
$RACERSTARTSCRIPT >/dev/null 2>&1 &

exit 0
