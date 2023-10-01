main : Bignum.cpp main.cpp
	clang++ -std=c++17 -Wall $? -o $@
	./$@
	rm -f ./$@