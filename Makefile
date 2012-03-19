CFLAGS = -std=c++0x -O2
LFLAGS = 
test: test.o
	g++ -o test test.o $(LFLAGS)
test.o: test.cpp algo.cpp graph.hpp
	g++ -c test.cpp $(CFLAGS)
clean:
	rm -f *.o test
