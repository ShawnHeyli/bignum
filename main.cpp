#include <iostream>
#include "Bignum.hpp"

using namespace std;

int main(void) {
  Bignum b1 = Bignum("31277893719");
  Bignum b2 = Bignum("1");

  // Write some tests
  cout << b1 << endl;
  cout << b1 + b2 << endl;
  cout << b1-b2 << endl;

  cout << (b1 > b2) << endl;
  cout << (b1 < b2) << endl;
  cout << (b1 >= b2) << endl;
  cout << (b1 <= b2) << endl;
  cout << (b1 == b2) << endl;
  cout << (b1 != b2) << endl;

  return 0;
}