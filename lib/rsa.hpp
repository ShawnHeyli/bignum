#ifndef _RSA
#define _RSA

#include "Bignum.hpp"
#include <utility>
#include <vector>
using namespace std;

// Keys

struct PublicKey {
  Bignum e;
  Bignum n;
};

struct PrivateKey {
  Bignum d;
  Bignum n;
};

bool witness_miller_rabin(Bignum const &, Bignum const &);
bool primality_miller_rabin(Bignum const &, uint const &k=50);
bool primality_fermat(Bignum const &, uint const &k=50);

Bignum random_bignum(int const &nbit=1024);
Bignum random_prime(int const &nbit=1024);

pair<PublicKey, PrivateKey> keygen(Bignum const &p, Bignum const &q);

vector<Bignum> encode(string const &);
string decode(vector<Bignum> const &);

vector<Bignum> encrypt(string const &, PublicKey);
string decrypt(vector<Bignum> const &, PrivateKey);

void rsa(int const &key_bitsize, string const &message);

#endif
