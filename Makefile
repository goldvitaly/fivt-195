digit: test.o
	g++ -o digit test.o
test.o: test.cpp digit.h
	g++ -Wall -c test.cpp
clean:
	rm -f digit *.o
