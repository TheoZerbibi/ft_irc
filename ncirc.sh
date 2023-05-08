#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 SERVER PORT"
	exit 1
fi

SERVER=$1
PORT=$2

nc $SERVER $PORT << EOF
NICK
USER isaac newton localhost :Isaac Newton
EOF
