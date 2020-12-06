import random
import copy
from random import choice
from string import ascii_uppercase

randstring = lambda x=125: ''.join(choice(ascii_uppercase) for i in range(x))

if __name__ == "__main__":

    count_of_tests = 4
    max_count_of_commands = 90000
    count_of_commands = 10000


    actions = ["+", "!", "?", "!"] # + - ? !
    for test_number in range(1, count_of_tests+1):
        tree_elems = {}
        saved = 0
        saved_file = {}
        test_file = open("tests/" + str(test_number) + ".t", 'w')
        file_with_answers = open("tests/" + str(test_number) + ".txt", "w")

        for _ in range( count_of_commands ):
            action = random.choice( actions )
            if action == "+":
                key = randstring()
                value = random.randint( 1, 2**64-1 )
                test_file.write("+ " + key + " " + str(value) + "\n")

                key = key.lower()
                answer = "Exist"
                if key not in tree_elems:
                    answer = "OK"
                    tree_elems[key] = value
                file_with_answers.write(answer + "\n")

            elif action == "-":
                key = randstring()
                test_file.write("- " + key + "\n")

                key = key.lower()
                answer = "NoSuchWord"
                if key in tree_elems:
                    del tree_elems[key]
                    answer = "OK"
                file_with_answers.write(answer + "\n")

            elif action == "?":
                search_exist_element = random.choice( [ True, False ] )
                key = random.choice([key for key in tree_elems.keys()]) if search_exist_element and len(tree_elems.keys()) > 0 else randstring()
                test_file.write(key + "\n")

                key = key.lower()
                if key in tree_elems:
                    answer = "OK: " + str(tree_elems[key])
                else:
                    answer = "NoSuchWord"
                file_with_answers.write(answer + "\n") 
            
            elif action == "!":
                act_file = random.choice(["Load test", "Save test"])
                if act_file == "Save test":
                    test_file.write(action + " " + act_file + "\n")
                    saved_file = tree_elems.copy()
                    file_with_answers.write("OK" + "\n")
                    saved = 1

                elif saved == 1 and act_file == "Load test":
                    test_file.write(action + " " + act_file + "\n")
                    tree_elems = {}
                    tree_elems = saved_file.copy()
                    file_with_answers.write("OK" + "\n")

        count_of_commands*=2
