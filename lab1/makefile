all:
	python tests_creator.py
	g++ benchmark.cpp 
	./a.out < test0.txt > answer10.txt
	./a.out < test0.txt > answer11.txt
	./a.out < test0.txt > answer12.txt
	./a.out < test0.txt > answer13.txt
	g++ main.cpp
	./a.out < test0.txt > answer20.txt
	./a.out < test0.txt > answer21.txt
	./a.out < test0.txt > answer22.txt
	./a.out < test0.txt > answer23.txt
	diff answer10.txt answer20.txt
	diff answer11.txt answer21.txt
	diff answer12.txt answer22.txt
	diff answer13.txt answer23.txt	