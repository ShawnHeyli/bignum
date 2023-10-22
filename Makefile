.PHONY: clean

all: bin/main bin/main_debug

bin/main : src/Bignum.cpp src/main.cpp
	clang++ -std=c++17 -Wall $^ -o $@

bin/main_debug : src/Bignum.cpp src/main.cpp
	clang++ -std=c++17 -g -Wall $^ -o $@

clean: 
	rm -rf bin/*