#!/bin/sh

if [ "$1" = "random" ]; then
	python server.py 10000 -n 5 -NC 2 -TL 150 -LOG server.log &
	python client.py 0.0.0.0 10000 RandomPlayer.py -mode GUI > client1.log &
	python client.py 0.0.0.0 10000 RandomPlayer.py > client2.log &
elif [ "$1" = "game" ]; then
	python ../server.py 10000 -n 5 -NC 2 -TL 150 -LOG ../server.log &
	python ../client.py 0.0.0.0 10000 ../RandomPlayer.py -mode GUI > ../client1.log &
	python ../client.py 0.0.0.0 10000 ../RandomPlayer.py > ../client2.log &
elif [ "$1" = "clean" ]; then
	# NAME=Check
	# echo $NAME		
	_PID=$(lsof -t -i :10000)
	if [ ${#_PID[*]} != 0 ]; then
		kill _PID
	fi
fi
