
A = 1
B = 701

number_of_tests = 500
delta = (B - A) // number_of_tests

for i in range(number_of_tests):
	file = open(f"tests/test{i}.txt", "w")
	file.write(str(A + delta * i))

	file.close()