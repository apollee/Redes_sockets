f = open('tests/test3.txt', "w")
f.write("reg 89526\n")
for i in range(1,100000):
	f.write("tl\n")
f.close