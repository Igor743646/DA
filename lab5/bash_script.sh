#!/bin/bash
max=100

echo ' ' > log.txt;
for ((i = 0; i < ${max}; i++)) 
do 
	echo "test: $i ";
	./main2.out < tests/test${i}.txt >> log.txt;
done
