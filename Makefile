.PHONY: clean

all: bin/main bin/main_debug

bin/main : src/Bignum.cpp src/rsa.cpp src/main.cpp
	g++ -std=c++17 -O3 -Wall -Ilib $^ -o $@ 

bin/main_debug : src/Bignum.cpp src/rsa.cpp src/main.cpp
	g++ -std=c++17 -gdwarf-4 -Wall -Ilib $^ -o $@

clean: 
	rm -rf bin/*