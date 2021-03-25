#!/bin/bash
max=100

for ((i = 0; i < ${max}; i++)) 
do 
	echo "test: $i ";
	./main.out < tests/test${i}.txt > tests/${i}ans.txt;
	echo "ans: $i "; 
	diff tests/${i}ans.txt tests/test${i}_ans.txt
done
