import random
import copy
from random import choice
from string import ascii_letters

randletter = lambda x=16: ''.join(choice(ascii_letters) for i in range(random.randint(0, x)))

def randstring():
	res = ""
	for _ in range(random.randint(1, 50)):
		res += randletter() + " "

	return res

if __name__ == "__main__":

	file = open("test0.txt", "w")

	pattern = ""
	for _ in range(random.randint(2, 9)):
		pattern += randletter() + " "

	file.write(pattern + "\n")

	for _ in range(random.randint(1, 10**2)):
		file.write(randstring() + choice([0,0,0,0,0,0,0,0,0,0,0, 1]) * pattern)


	file.close()