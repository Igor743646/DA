import random as r

OPERATIONS = ["^"] #, "-", "*", "/", "<", ">", "=", "^"]

def randomint(n = 10):
	s = r.randint(1, 10)
	for i in range(n-1):
		s = s*10 + r.randint(0, 10)
	return s

def operate(a, b, op):
	if op == "+":
		return a+b
	elif op == "-":
		return "Error" if a < b else a-b
	elif op == "*":
		return a*b
	elif op == "/":
		return "Error" if b == 0 else a//b
	elif op == "^":
		return "Error" if a == 0 and b == 0 else a**b
	elif op == "<":
		return "true" if a < b else "false"
	elif op == ">":
		return "true" if a > b else "false"
	elif op == "=":
		return "true" if a == b else "false"



file = open(f"r.txt", "w")
for j in range(100):

	file_test = open(f"tests/test{j}.txt", "w")
	file_answer = open(f"tests/test{j}_ans.txt", "w")

	for i in range(5):

		a, b, op = randomint(1+j*4), randomint(1+j*4), r.choice(OPERATIONS)

		if op == "^":
			b = randomint(2)

		if op == "/":
			b = randomint(j//2+1)

		file_test.write(f"{a}\n{b}\n{op}\n")
		file_answer.write(f"{operate(a, b, op)}\n")

	file_test.close()
	file_answer.close()

	file.write(f"{1+4*j}\n")

file.close()