#!/bin/bash
CC = g++
FLAGS = -pedantic -Wall -std=c++11 -Werror -Wno-sign-compare -O2 -lm

all: main.out

main.out: main.o
	${CC} ${FLAGS} main.o -o main.out

%.o: %.cpp
	${CC} ${FLAGS} -c $<

run:
	./main.out

runv:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./main.out

clean:
	rm -rf *.out *.o
