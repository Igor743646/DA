#!/bin/bash
max=100

for ((i = 0; i < ${max}; i++)) 
do 
	echo "test: $i ";
	./bench.out < tests/test${i}.txt >> tests/ans.txt;
done
