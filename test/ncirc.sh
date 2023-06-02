#!/bin/bash

./proxy/simple-tcp-proxy 1025 &> output &
PID=$?

function exit_properly()
{
	if [ -e pipo ]
	then
		rm -f pipo
	fi

	kill -9 $1
	exit
}

trap "exit_properly $PID" SIGINT

trap -p SIGINT

if [ ! -e pipo ]
then
	mkfifo pipo
fi

nc localhost 1025 < pipo &

echo "nbr of argument $#"

if [ $# -gt 0 ]
then
	cat "$1" - > pipo
else
	cat > pipo
fi

rm -f pipo

kill -9 pid
