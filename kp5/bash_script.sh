#!/bin/bash
max=500

function check_dir {
	if [ -d "$1" ]; then
		echo "$1 is present"
	else
		echo "$1 is not present. Creating..."
		mkdir -p $1
	fi
}

function check_file {
	if [ -f "$1" ]; then
		echo "$1 is present"
	else
		echo "$1 is not present. Creating..."
		make
	fi
}

check_dir "tests"
check_file "prog2.out"

echo "generate tests...";
#python3 tester2.py ${max};

echo "program execution...";
for ((i = 0; i < ${max}; i++)) 
do 
	echo "test: $i ";
	#./prog2.out index --input tests/test${i}.txt --output tests/saved_test${i}.txt;
	./prog2.out search --index tests/saved_test${i}.txt --input tests/test_points${i}.txt --output tests/ans_test${i}.txt
	#diff tests/ans_test${i}.txt tests/test_ans${i}.txt;

done