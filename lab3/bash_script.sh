#!/bin/bash
max=101

for ((i = 1; i < ${max}; i++)) 
do 
	echo "test: $i ";
	./main.out < tests/${i}.t > tests/${i}ans.txt;
	echo "ans: $i "; 
	diff tests/${i}ans.txt tests/${i}.txt
done
