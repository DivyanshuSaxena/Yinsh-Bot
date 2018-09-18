#!/bin/bash

## Compile our two program files
g++ -std=c++11 -Ofast -march=native -o code Yinsh-Bot/main.cpp Yinsh-Bot/Board.cpp Yinsh-Bot/State.cpp Yinsh-Bot/TimeHelper.cpp -I./Yinsh-Bot/ -g
