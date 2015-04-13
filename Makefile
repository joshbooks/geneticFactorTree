build: debug

debug: src/FactorTree.cpp src/main.cpp src/Population.cpp
	g++ -Wall -fexceptions -std=c++11 -g -I/home/flatline/cbwork/factorTree/ -c /home/flatline/cbwork/factorTree/src/FactorTree.cpp -o obj/Debug/src/FactorTree.o
	g++ -Wall -fexceptions -std=c++11 -g -I/home/flatline/cbwork/factorTree/ -c /home/flatline/cbwork/factorTree/src/main.cpp -o obj/Debug/src/main.o
	g++ -Wall -fexceptions -std=c++11 -g -I/home/flatline/cbwork/factorTree/ -c /home/flatline/cbwork/factorTree/src/Population.cpp -o obj/Debug/src/Population.o
	g++  -o bin/Debug/factorTree obj/Debug/src/FactorTree.o obj/Debug/src/main.o obj/Debug/src/Population.o   -lgmp

clean:
	rm -rf ./bin
