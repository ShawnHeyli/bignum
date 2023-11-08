#include <iostream>

#include "../lib/Bignum.hpp"

using namespace std;

int main(void) {

  cout << "TEST ADDITION" << endl;
  cout << "3 + 2 (expected: 5) = " << Bignum(3) + Bignum(2) << endl;
  cout << "73418 + 267 (expected: 73685) = " << Bignum(73418) + Bignum(267) << endl;
  cout << "4294967297 + 2 (expected: 4294967299) = " << Bignum("4294967297") + Bignum(2) << endl;

  cout << endl << "TEST SOUSTRACTION" << endl;
  cout << "3-2 (expected: 1) = " << Bignum(3) - Bignum(2) << endl;
  cout << "73418-267 (expected: 73151) = " << Bignum(73418) - Bignum(267) << endl;
  cout << "4294967297 - 2 (expected: 4294967295) = " << Bignum("4294967297") - Bignum(2) << endl;

  cout << endl << "TEST MULTIPLICATION" << endl;
  cout << "3*2 (expected: 6) = " << Bignum(3) * Bignum(2) << endl;
  cout << "73418*267 (expected: 19602606) = " << Bignum(73418) * Bignum(267) << endl;
  cout << "4294967297 * 2 (expected: 8589934594) = " << Bignum("4294967297") * Bignum(2) << endl;

  cout << endl << "TEST DIVISION" << endl;
  cout << "3/2 (expected: 1) = " << Bignum(3) / Bignum(2) << endl;
  cout << "73418/267 (expected: 274) = " << Bignum(73418) / Bignum(267) << endl;
  cout << "4294967297 / 2 (expected: 2147483648) = " << Bignum("4294967297") / Bignum(2) << endl;

  cout << endl <<  "TEST `MODULO" << endl;
  cout << "3%2 (expected: 1) = " << Bignum(3) % Bignum(2) << endl;
  cout << "73418/267 (expected: 260) = " << Bignum(73418) % Bignum(267) << endl;
  cout << "4294967297 % 2000000 (expected: 967297) = " << Bignum("4294967297") % Bignum("2000000") << endl;

  cout << endl << "TEST INVERSE MODULAIRE" << endl;
  cout << "inverseMod(3, 11) (expected: 4) = " << inverseMod(Bignum(3), Bignum(11)) << endl;
  cout << "inverseMod(17, 59) (expected: 7) = " << inverseMod(Bignum(17), Bignum(59)) << endl;

  cout << endl << "TEST EXPONENTIATION MODULAIRE" << endl;
  cout << "expMod(3, 2, 5) (expected: 4) = " << modPow(Bignum(3), Bignum(2), Bignum(5)) << endl;
  cout << "expMod(3, 3, 5) (expected: 2) = " << modPow(Bignum(3), Bignum(3), Bignum(5)) << endl;
  cout << "expMod(7890, 25, 11) (expected: 1) = " << modPow(Bignum(7890), Bignum(25), Bignum(11)) << endl;
  
  
  return 0;
}