test.bin: test.o
	g++ -o test.bin test.o
test.o: test.cpp interval.h
	g++ -O0 -c test.cpp
clean:
	rm -f *.bin *.o
