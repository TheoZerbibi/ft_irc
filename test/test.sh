#!/bin/bash



#	mkfifo pipe
#	
#	nc localhost 1024 < pipe  &
#	
#	while true
#	do
#		echo "NICK test" > pipe
#	done
#	
#	rm pipe

cat $1
