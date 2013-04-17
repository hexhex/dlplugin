# $1: timeout

if [ $# -le 0 ]; then
	to=300
else
	to=$1
fi

	echo "
		Executable = ./benchmark_single.sh
		Universe = vanilla
		output = birdpenguin.out
		error = birdpenguin.error
		Log = birdpenguin.log
		Requirements = machine == \"node5.kr.tuwien.ac.at\"
		request_memory = 8192 
		Initialdir = $PWD
		notification = never

		# queue
		request_cpus = 1 
		Arguments = $PATH $LD_LIBRARY_PATH 20 $to 
		Queue 1
	     " > p.job
	condor_submit p.job

