binary:objects
	g++ -o bin/binary obj/*.o
	
objects:src/*/*.cpp
	g++ -c src/*/*.cpp -I. -Isrc
	mv *.o obj

clean:
	rm -r -f obj/*
	rm -r -f bin/*
	
