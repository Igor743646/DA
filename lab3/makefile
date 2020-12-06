#!/bin/bash
CC = g++
FLAGS = -pedantic -Wall -std=c++11 -Werror -Wno-sign-compare -O2 -lm

all: main.out

main.out: main.cpp avltree.hpp string.hpp
	${CC} ${FLAGS} main.cpp -o main.out

run:
	rm -rf time_log.txt
	python tests_creator.py
	./bash_script.sh
	gnuplot gnuplot_script.txt

testss:
	python tests_creator.py