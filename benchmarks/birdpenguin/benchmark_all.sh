# $1: timeout

if [ $# -le 0 ]; then
	to=300
else
	to=$1
fi

	echo "
		Executable = ./benchmark.sh
		Universe = vanilla
		output = birdpenguin.out
		error = birdpenguin.error
		Log = birdpenguin.log
		Requirements = machine == \"lion.kr.tuwien.ac.at\"
		request_memory = 4096 
		Initialdir = $PWD/
		notification = never

		# queue
		request_cpus = 1 
		Arguments =
		Queue 1
	     " > p.job
	condor_submit p.job

