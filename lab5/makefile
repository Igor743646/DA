#!/bin/bash
CC = g++
FLAGS = -pedantic -Wall -fpermissive -std=c++11 -Werror -Wno-sign-compare -O2 -lm

all: main.out main2.out

main.out: main.o
	${CC} ${FLAGS} main.o -o main.out

main2.out: main2.o
	${CC} ${FLAGS} main2.o -o main2.out

%.o: %.cpp
	${CC} ${FLAGS} -c $<

run:
	bash bash_script.sh
	gnuplot gnuplot_script.txt

runv:
	python3 tester.py
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./main2.out < tests/test0.txt

clean:
	rm -rf *.out *.o
