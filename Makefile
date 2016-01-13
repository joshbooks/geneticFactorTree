build: debug

debug: src/FactorTree.cpp src/main.cpp src/Population.cpp
	g++ -Wall -fexceptions -std=c++11 -g -c src/FactorTree.cpp -o FactorTree.o
	g++ -Wall -fexceptions -std=c++11 -g -c src/main.cpp -o main.o
	g++ -Wall -fexceptions -std=c++11 -g -c src/Population.cpp -o Population.o
	g++  -o factorTree FactorTree.o main.o Population.o  -lgmp

clean:
	rm -rf ./bin
