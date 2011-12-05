sources=$(wildcard src/*.cpp)
headers=$(wildcard src/*.h)
objects=$(sources:src/%.cpp=obj/%.o )

bin/testinput.txt: bin/binary
	cp tests/* bin/
	python bin/gen.py

bin/binary: $(objects) bin
	g++ -o bin/binary $(objects)

$(objects): obj/%.o: src/%.cpp $(headers) obj
	g++ -o $@ -c $<

obj:
	mkdir obj
	
bin:
	mkdir bin
	
clean:
	rm -f -r obj
	rm -f -r bin
	
