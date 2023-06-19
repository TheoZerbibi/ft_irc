#!/bin/bash

./proxy/simple-tcp-proxy 1025 | tee output &
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

function autopong()
{
	while 1
	do
		read -r line
		echo "line = $line"
		if [[ $line == "PING "* ]]
		then
			ping_code="${line#PING }"
			response="PONG $ping_code"
			echo "$response" > pipo
		fi
	done < output
}


trap "exit_properly $PID" SIGINT

trap -p SIGINT

if [ ! -e pipo ]
then
	mkfifo pipo
fi

sleep 1


nc localhost 1025 < pipo &> /dev/null &

autopong &

echo "nbr of argument $#"

if [ $# -gt 0 ]
then
	cat "$1" - > pipo
else
	cat > pipo
fi
