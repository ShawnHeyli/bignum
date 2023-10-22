#include <iostream>

#include "../lib/Bignum.hpp"

using namespace std;

int main(void) {
  cout << "3/2 (expected: 1) = " << Bignum(3) / Bignum(2);
  cout << "73418/267 (expected: 274) = " << Bignum(73418) / Bignum(267);
  cout << "4294967297 / 2 (expected: 2147483648) = " << Bignum("4294967297") / Bignum(2);
  return 0;
}