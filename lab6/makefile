#!/bin/bash
CC = g++
FLAGS = -pedantic -Wall -fpermissive -std=c++11 -Werror -Wno-sign-compare -O2 -lm

all: main.out

main.out: main.o
	${CC} ${FLAGS} main.o -o main.out

%.o: %.cpp
	${CC} ${FLAGS} -c $<

run:
	python3 tester.py
	bash bash_script.sh
	gnuplot gnuplot_script.txt
	rm -f time_log.txt

runv:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./main.out < tests/test0.txt

clean:
	rm -rf *.out *.o
