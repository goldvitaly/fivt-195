from random import randint
f = open('bin/testinput.txt', 'w')
for i in range(50):
	f.write(str(randint(1,100000000))+' ')
f.close()
