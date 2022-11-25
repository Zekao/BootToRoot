import itertools

stuff = ["1", "2", "3", "5", "6"]
for index, subset in enumerate(itertools.permutations(stuff)):
    with open("test_set{}.txt".format(index), "w") as f:
        f.write("""Public speaking is very easy.
1 2 6 24 120 720
1 b 214
9
opekmq\n""")
        f.write("4 " + " ".join(subset) + "\n")
