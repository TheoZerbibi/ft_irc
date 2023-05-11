#!/bin/bash

if [ "$#" -ne 2 ]; then
	SERVER=localhost
	PORT=1024
else
	SERVER=$1
	PORT=$2
fi



./proxy/simple-tcp-proxy "$PORT" 6667 &
PID=$?

nc "$SERVER" "$PORT" << EOF
NICK isaac
USER isaac newton localhost :Isaac Newton
EOF
