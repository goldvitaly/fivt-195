headers=src/general.h src/node.h src/binheap.h
sources=src/node.cpp src/binheap.cpp
objects=obj/binheap.o obj/node.o

test:test/test.cpp obj Makefile
	cp test/test.cpp src
	g++ -c src/test.cpp -o obj/test.o
	rm src/test.cpp
	g++ -o bin/test $(objects) obj/test.o

obj:$(sources) $(headers)  Makefile 
	g++ -c $(sources)
	mv *.o obj

clean:
	rm obj/*
	rm bin/*
