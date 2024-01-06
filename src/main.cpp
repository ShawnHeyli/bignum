#include <cstdlib>
#include <iostream>

#include "../lib/Bignum.hpp"
#include "../lib/rsa.hpp"

using namespace std;

int main(void) {

  srand(time(NULL));

  string message = "Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet.";

  int nbit = 256;
  auto p = random_prime(nbit);
  auto q = random_prime(nbit);
  cout << "p = " << p << endl;
  cout << "q = " << q << endl;
  Bignum n = p * q;
  Bignum phi = (p - 1) * (q - 1);
  Bignum e = 65537;
  Bignum d = invMod(e, phi);

  auto cypher = encrypt(message, PublicKey{e, n});
  auto plaintext = decrypt(cypher, PrivateKey{d, n});
  
  cout << "Encrypted message: " << plaintext << endl;

  //rsa(512, "Hi mom!");
  
  exit(0);

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

  cout << endl <<  "TEST MODULO" << endl;
  cout << "3%2 (expected: 1) = " << Bignum(3) % Bignum(2) << endl;
  cout << "73418/267 (expected: 260) = " << Bignum(73418) % Bignum(267) << endl;
  cout << "4294967297 % 2000000 (expected: 967297) = " << Bignum("4294967297") % Bignum("2000000") << endl;

  cout << endl << "TEST INVERSE MODULAIRE" << endl;
  cout << "invMod(123, 4567) (expected: 854) = " << invMod(Bignum(123), Bignum(4567)) << endl;
  cout << "invMod(93617137, 37932134) (expected: 36007025) = " << invMod(Bignum("93617137"), Bignum("37932134")) << endl;

  cout << endl << "TEST EXPONENTIATION MODULAIRE" << endl;
  cout << "expMod(3, 2, 5) (expected: 4) = " << modPow(Bignum(3), Bignum(2), Bignum(5)) << endl;
  cout << "expMod(3, 3, 5) (expected: 2) = " << modPow(Bignum(3), Bignum(3), Bignum(5)) << endl;
  cout << "expMod(170141183460469231731687303715884105727, 25, 12) (expected: 7) = " << modPow(Bignum("170141183460469231731687303715884105727"), Bignum(25), Bignum(12)) << endl;
  cout << "expMod(170141183460469231731687303715884105727, 170141183460469231731687303715884105727, 325) (expected: 153) = " << modPow(Bignum("170141183460469231731687303715884105727"), Bignum("170141183460469231731687303715884105727"), Bignum("325")) << endl;
  
  cout << endl << "TEST PRIMALITY MILLER RABIN" << endl;
  cout << "miller_rabin_test(561) (expected: false) = " << primality_miller_rabin(Bignum("561")) << endl;
  cout << "miller_rabin_test(1373653) (expected: false) = " << primality_miller_rabin(Bignum("1373653")) << endl;
  cout << "miller_rabin_test(67280421310721) (expected: true) = " << primality_miller_rabin(Bignum("67280421310721")) << endl;
  cout << "miller_rabin_test(1635795965187779) (expected: true) = " << primality_miller_rabin(Bignum("1635795965187779")) << endl;
  cout << "miller_rabin_test(16357937173713) (expected: false) = " << primality_miller_rabin(Bignum("16357937173713")) << endl;
  
  cout << endl << "TEST PRIMALITY FERMAT" << endl;
  cout << "primality_fermat(561) (expected: false) = " << primality_fermat(Bignum("561")) << endl;
  cout << "primality_fermat(1373653) (expected: false) = " << primality_fermat(Bignum("1373653")) << endl;
  cout << "primality_fermat(67280421310721) (expected: true) = " << primality_fermat(Bignum("67280421310721")) << endl;
  cout << "primality_fermat(1635795965187779) (expected: true) = " << primality_fermat(Bignum("1635795965187779")) << endl;
  cout << "primality_fermat(16357937173713) (expected: false) = " << primality_fermat(Bignum("16357937173713")) << endl;

  cout << endl << "TEST RANDOM BIGNUM" << endl;
  cout << "random_bignum(32 bits) = " << random_bignum(1) << endl;
  cout << "random_bignum(64 bits) = " << random_bignum(2) << endl;
  cout << "random_bignum(128 bits) = " << random_bignum(4) << endl;
  cout << "random_bignum(256 bits) = " << random_bignum(8) << endl;
  cout << "random_bignum(512 bits) = " << random_bignum(16) << endl;
  cout << "random_bignum(1024 bits) = " << random_bignum(32) << endl;

  cout << endl << "RANDOM PRIME" << endl;
  cout << "random_prime(4 bits) = " << random_prime(1) << endl;

  cout << endl << "KEYGEN RSA" << endl;
  pair<PublicKey, PrivateKey> keys = keygen(Bignum("67280421310721"), Bignum("1635795965187779"));
  cout << "e = " << keys.first.e << endl;
  cout << "n = " << keys.first.n << endl;
  cout << "d = " << keys.second.d << endl;
  cout << "n = " << keys.second.n << endl;

  return 0;
}