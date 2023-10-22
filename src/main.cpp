#include <iostream>

#include "../lib/Bignum.hpp"

using namespace std;

int main(void) {
  cout << "TEST DIVISION" << endl << endl;
  cout << "3/2 (expected: 1) = " << Bignum(3) / Bignum(2) << endl;
  cout << "73418/267 (expected: 274) = " << Bignum(73418) / Bignum(267) << endl;
  cout << "4294967297 / 2 (expected: 2147483648) = " << Bignum("4294967297") / Bignum(2) << endl;

  cout << endl <<  "TEST `MODULO" << endl << endl;
  cout << "3%2 (expected: 1) = " << Bignum(3) % Bignum(2) << endl;
  cout << "73418/267 (expected: 260) = " << Bignum(73418) % Bignum(267) << endl;
  cout << "4294967297 % 2000000 (expected: 967297) = " << Bignum("4294967297") % Bignum("2000000") << endl;

  //cout << endl << "TEST INVERSE MODULAIRE" << endl << endl;
  //cout << "inverseMod(3, 11) (expected: 4) = " << inverseMod(Bignum(3), Bignum(11)) << endl;

  return 0;
}