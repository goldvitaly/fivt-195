test: test.o
	g++ -o test test.o
test.o: test.cpp interval.h
	g++ -O0 -c test.cpp
clean:
	rm -f test *.o
