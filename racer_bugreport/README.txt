"$ foo" indicates to execute the command foo on the console

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


