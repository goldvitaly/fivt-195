from random import randint
f = open('bin/testinput.txt', 'w')
for i in range(30000):
	f.write(str(randint(1,10000))+' ')
f.close()
