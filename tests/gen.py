from random import randint
f = open('bin/testinput.txt', 'w')
i = 1
while i < 1000000:
	f.write(str(randint(1,100000000)/100.0))
	f.write(' ')
	i += 1
f.close()

