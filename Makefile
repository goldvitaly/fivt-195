CFLAGS = -std=c++0x -O2 -Wall
LFLAGS = 
test: test.o
	g++ -o test test.o $(LFLAGS)
test.o: test.cpp base.hpp dfs.hpp graph.hpp kosaraju.hpp tarjan.hpp
	g++ -c test.cpp $(CFLAGS)
clean:
	rm -f *.o test
