#!/bin/bash

RACER=RacerPro
RACERSTARTSCRIPT=RacerPro.sh

# make sure to kill it, especially if it is hanging
killall -9 $RACER

# start racer in a screen called 'racer' in detached mode
# connect to it via "screen -x racer"
screen -S racer -d -m $RACERSTARTSCRIPT

exit 0
