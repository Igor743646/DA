from random import *

A = 1
B = 300001

number_of_tests = 100
delta = (B - A) // number_of_tests

for i in range(number_of_tests):
	file = open(f"tests/test{i}.txt", "w")

	n = A + delta*i;
	m = A + delta*i;

	file.write(f'{n} {m}\n')

	for _ in range(m):
		file.write(f"{randint(1, n)} {randint(1, n)}\n");

	file.close()