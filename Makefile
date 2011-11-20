headers=src/general.h src/node.h src/binheap.h
sources=src/node.cpp src/binheap.cpp
obj:$(sources) Makefile
	g++ -c $(sources)
	mv *.o obj
test:obj Makefile
	cp test/test.cpp src
	g++ -c src/test.cpp -o obj/test.o
	rm src/test.cpp
	g++ -o bin/test obj/test.o
