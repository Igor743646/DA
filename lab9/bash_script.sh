#!/bin/bash
max=100

rm -rf tests/ans1.txt tests/ans2.txt tests/ans3.txt;
for ((i = 0; i < ${max}; i++)) 
do 
	echo "test: $i ";
	./bench.out < tests/test${i}.txt >> tests/ans1.txt;
	./bench.out < tests/test${i}.txt >> tests/ans2.txt;
	./bench.out < tests/test${i}.txt >> tests/ans3.txt;
done
