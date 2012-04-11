main: main.cpp graph.hpp incidence.hpp vector_incidence.hpp set_incidence.hpp tarjan.hpp tarjan.cpp
	g++ -O2 -Wall -std=c++0x main.cpp tarjan.cpp -o main
