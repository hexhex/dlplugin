# -*- org -*-

"$ foo" indicates to execute the command foo on the console

* Bug 1

On one console:

start RacerPro-1-9-2-beta (nixon.owl must be in the directory where you start racer):
$ ./RacerPro

OR

start RacerPro-2-0-Preview-Linux32 (nixon.owl must be in the directory where you start racer):
$ ./RacerPro -ef cr

On another console:

The last query in this file works:
$ cat nixon_goodcase.racer |netcat localhost 8088

The (same) last query in this file does not work,
because the 'retrieve-under-premise' corrupts the loaded ontology:
$ cat nixon_bugcase.racer |netcat localhost 8088


* Bug 2 (Racer hangs forever, only confirmed on x86_64 machines so far)

start RacerPro 1.9.0
$ ./RacerPro

then call 10-20 times
$ netcat localhost 8088 < racer-bad-cmd-seq.racer

in every good run, netcat prints 
:answer 1 "NIL" ""
:answer 2 ":OKAY-FULL-RESET" ""
:answer 3 "T" ""
:answer 4 "|file:shop.owl|" "	Reading shop.owl...	done.	"
:answer 5 "(|http://www.kr.tuwien.ac.at/staff/roman/shop#shop|)" ""
:answer 6 "KB-1773215" ""
:answer 7 "NIL" ""
:ok 8 ""
:answer 9 "T" ""
:answer 10 ":OKAY-FULL-RESET" ""
:answer 11 "T" ""
:answer 12 "|file:shop.owl|" "	Reading shop.owl...	done.	"
:answer 13 "(|http://www.kr.tuwien.ac.at/staff/roman/shop#shop|)" ""
:answer 14 "KB-1773215" ""
:error 15 ABox KB-1773215 is incoherent. ""
:ok 16 ""

and then quits. In a bad run, netcat hangs after printing :answer 14
"KB-1773215" "". Killing netcat and then running netcat again results
in no answer from RacerPro and an immediate hang.
