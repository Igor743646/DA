from random import *
from string import *

OPERATIONS = ["^"] #, "-", "*", "/", "<", ">", "=", "^"]

def randstring(n = 10):
	result = ""
	for i in range(randint(3, n)):
		result += choice(ascii_lowercase)
	return result

def sstring(n):
	result = ""
	for i in range(n):
		result += choice(ascii_lowercase)
	return result


K = 100
A = 10



str_text = sstring(300000)

B = len(str_text)
dl = (B - A) // K

for j in range(K):

	file_test = open(f"tests/test{j}.txt", "w")
	text = str_text + '\n'#sstring(A + (dl * j)) + '\n'
	file_test.write(text)

	for _ in range(200):

		if choice([1]) == 1:
			a = randint(0, len(text) - (A + (dl * j)) - 1) #, randint(0, A + (dl * j))
			b = a + (A + (dl * j))
			if a > b:
				a, b = b, a
			elif a == b:
				a -= 1

			pattern = text[a : b]
		else: 
			pattern = randstring() 


		file_test.write(pattern + '\n')

	file_test.close()

