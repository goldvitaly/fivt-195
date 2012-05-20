CFLAGS = -std=c++0x -Wall -O2
LFLAGS = 
dist/test: dist/libgraph.a
	cd dist && g++ -o test test.cpp -Iinclude -L. -lgraph $(CFLAGS)
dist/libgraph.a: base.o dfs.o kosaraju.o tarjan.o graph.o adapter.hpp dijkstra.hpp test.cpp weighted.hpp dijkstra_util.hpp path.hpp
	mkdir -p dist
	mkdir -p dist/include
	cp *.hpp dist/include
	ar rcs libgraph.a base.o dfs.o kosaraju.o tarjan.o graph.o
	mv libgraph.a dist
	cp test.cpp dist
graph.o: graph.hpp graph.cpp
	g++ -c graph.cpp $(CFLAGS)
base.o: base.hpp base.cpp
	g++ -c base.cpp $(CFLAGS)
dfs.o: dfs.hpp dfs.cpp
	g++ -c dfs.cpp $(CFLAGS)
kosaraju.o: kosaraju.hpp kosaraju.cpp
	g++ -c kosaraju.cpp $(CFLAGS)
tarjan.o: tarjan.hpp tarjan.cpp path.hpp
	g++ -c tarjan.cpp $(CFLAGS)
distclean:
	rm -rf dist
clean: 
	rm -f *.o
.PHONY: clean
