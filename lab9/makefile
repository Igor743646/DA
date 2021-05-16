#!/bin/bash
CC = g++
FLAGS = -pedantic -Wall -fpermissive -std=c++11 -Werror -Wno-sign-compare -O2 -lm

all: main.out bench.out

%.out: %.o
	${CC} ${FLAGS} $< -o $@

%.o: %.cpp
	${CC} ${FLAGS} -c $<

run:
	rm -f tests/ans.txt
	python3 tester.py
	bash bash*.sh
	gnuplot gnu*

runv:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./bench.out

clean:
	rm -rf *.out *.o
