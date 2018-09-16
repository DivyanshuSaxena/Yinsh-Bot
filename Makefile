all: main.cpp TimeHelper.cpp Board.cpp State.cpp
	    g++ -o main main.cpp TimeHelper.cpp Board.cpp State.cpp -I./ -g
