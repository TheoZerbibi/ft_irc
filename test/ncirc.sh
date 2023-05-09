#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 SERVER PORT"
	exit 1
fi

SERVER=$1
PORT=$2


./proxy/simple-tcp-proxy "$PORT" 6667 &

nc "$SERVER" "$PORT" << EOF
NICK isaac
USER isaac newton localhost :Isaac Newton
EOF
