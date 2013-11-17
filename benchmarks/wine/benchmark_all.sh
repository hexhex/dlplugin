# $1: timeout

if [ $# -le 0 ]; then
	to=300
else
	to=$1
fi

	requirements=$(cat req 2> /dev/null)
	echo "
		Executable = ./benchmark_single.sh
		output = wine.out
		error = wine.error
		Log = wine.log
		$requirements
		Initialdir = $PWD
		notification = never

		# queue
		request_cpus = 1 
		Arguments = $PATH $LD_LIBRARY_PATH $to
		Queue 1
	     " > p.job
	condor_submit p.job

