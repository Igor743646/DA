from random import *

A = 1
B = 14001

number_of_tests = 100
delta = (B - A) // number_of_tests

for i in range(number_of_tests):
	file = open(f"tests/test{i}.txt", "w")

	file.write(f'{A + delta*i}\n')
	file.write(' '.join([str(randint(1,3)) for _ in range(A + delta*i)]))
	file.write('\n')
	file.close()