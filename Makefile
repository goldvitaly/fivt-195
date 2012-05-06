CFLAGS = -std=c++0x -Wall -O2
LFLAGS = 
test: test.o base.o dfs.o kosaraju.o tarjan.o
	g++ -o test test.o base.o dfs.o kosaraju.o tarjan.o $(LFLAGS)
test.o: test.cpp base.hpp graph.hpp dfs.hpp kosaraju.hpp tarjan.hpp weighted.hpp
	g++ -c test.cpp $(CFLAGS)
base.o: base.hpp base.cpp
	g++ -c base.cpp $(CFLAGS)
dfs.o: dfs.hpp dfs.cpp
	g++ -c dfs.cpp $(CFLAGS)
kosaraju.o: kosaraju.hpp kosaraju.cpp
	g++ -c kosaraju.cpp $(CFLAGS)
tarjan.o: tarjan.hpp tarjan.cpp
	g++ -c tarjan.cpp $(CFLAGS)
clean:
	rm -f *.o
