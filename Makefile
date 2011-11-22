bin:obj
	g++ -o bin/binary obj/*.o
	
obj:src/*/*.cpp
	g++ -c src/*/*.cpp -Isrc -Isrc/*
	mv *.o obj

clean:
	rm -r -f obj/*
	rm -r -f bin/*
	
