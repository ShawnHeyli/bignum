#ifndef _RSA
#define _RSA

#include "Bignum.hpp"
#include <utility>
#include <vector>
using namespace std;

struct PublicKey {
  Bignum e;
  Bignum n;
};

struct PrivateKey {
  Bignum d;
  Bignum n;
};

Bignum random_bignum(const int);
Bignum prime(const int);

pair<PublicKey, PrivateKey> keygen(Bignum const &, Bignum const &);

Bignum encode(string);
string decode(Bignum);
vector<Bignum> encode_vector(string, Bignum &, Bignum &);
string decode_vector(vector<Bignum> , Bignum &, Bignum &);

#endif
