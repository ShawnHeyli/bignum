#include "../lib/rsa.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// Doesn't work 
bool witness_miller_rabin(Bignum const &n, Bignum const &a) {
  if (n < 3)
    return false;
  if (n % 2 == 0)
    return false;
  if (a < 1)
    return false;

  Bignum d = n - 1;
  Bignum s = 0;
  while (d % 2 == 0) {
    d /= 2;
    s += 1;
  }

  Bignum x = modPow(a, d, n);
  if (x == 1 || x == n - 1)
    return false;

  for (uint i = 0; i < s - 1; i++) {
    x = (x ^ 2) % n;
    if (x == 1)
      return false;
    if (x == n - 1)
      return false;
  }

  return true;
}

// Doesn't work 
bool primality_miller_rabin(Bignum const &n, uint const &nb_test) {
    if (n < 3)
        return false;
    if (n % 2 == 0)
        return false;
    
    for (uint i = 0; i < nb_test; i++) {
        Bignum a = rand() % (n - 2) + 2;
        if (witness_miller_rabin(n, a)){
            return false;
        }
    }
    return true;
}

bool primality_fermat(Bignum const &n, uint const &nb_test) {
    if (n < 3)
        return false;
    if (n % 2 == 0)
        return false;
    
    for (uint i = 0; i < nb_test; i++) {
        Bignum a = rand() % (n - 2) + 2;
        if (modPow(a, n - 1, n) != 1)
            return false;
    }
    return true;
}

Bignum random_bignum(int const &nbit){
  int nbyte = nbit / 32;
  Bignum n = 0;
  n.isPositive = true;
  n.tab[0] = rand();
  if(n.tab[0] % 2 == 0){
    n.tab[0] = n.tab[0] + 1;
  }

  for (int i = 1; i < nbyte; i++ ) {
      n.tab.push_back(rand());
  }

  cout << "n = " << n << endl;
  return n;
}

// incremental search
// https://crypto.stackexchange.com/questions/1970/how-are-primes-generated-for-rsa
Bignum random_prime(int const &nbit){
  Bignum n = random_bignum(nbit);
  while(!primality_fermat(n, 20)){
    n = n + 2;
  }
  return n;
}

// Encodes one letter by Bignum for simplicity
// TODO make it more efficientby cramming 4 letters in one Bignum
vector<Bignum> encode (string const &input)
{
  vector<Bignum> output;
  for (unsigned long i = 0; i < input.size(); i++)
  {
    char c = input[i];
    Bignum tmp = Bignum(c);
    output.emplace_back(tmp);
  }
  return output;
}

string decode(vector<Bignum> const &input)
{
  string output = "";
  for (unsigned long i = 0; i < input.size(); i++){
    output += char(input[i].tab[0]);
  }    
  return output;
}

pair<PublicKey, PrivateKey> keygen(Bignum const &p, Bignum const &q){
    Bignum P = p;
    Bignum Q = q;
    if (p == q)
        throw "p and q must be different";
    if(!primality_fermat(p) || !primality_fermat(q))
        throw "p and q must be prime";
    Bignum n = P * Q;
    Bignum phi = (P - 1) * (Q - 1);
    Bignum e = 65537;
    Bignum d = invMod(e, phi);
    return make_pair(PublicKey{e, n}, PrivateKey{d, n});
}

vector<Bignum> encrypt(string const &message, PublicKey pk){
    vector<Bignum> output = encode(message);
    for (unsigned long i = 0; i < output.size(); i++)
    {
        output[i] = modPow(output[i], pk.e, pk.n);
    }
    return output;
}

string decrypt(vector<Bignum> const &cipher, PrivateKey sk){
    vector<Bignum> output = cipher;
    for (unsigned long i = 0; i < output.size(); i++)
    {
        output[i] = modPow(output[i], sk.d, sk.n);
    }
    return decode(output);
}

void rsa(int nbit, string message)
{
  auto start = chrono::system_clock::now();

  Bignum p = random_prime(nbit);
  Bignum q = random_prime(nbit);

  auto end = chrono::system_clock::now();
  auto elapsed = chrono::duration_cast<chrono::seconds>(end - start);
  cout << "p & q in: " << elapsed.count() << " seconds" <<'\n';

  pair<PublicKey, PrivateKey> keys = keygen(p, q);

  vector<Bignum> cypher = encrypt(message, keys.first);
  string plaintext = decrypt(cypher, keys.second);

  cout << "plaintext = " << plaintext << endl;
  cout << endl;

  end = chrono::system_clock::now();
  elapsed = chrono::duration_cast<chrono::seconds>(end - start);
  cout << "RSA in: " << elapsed.count() << " seconds" <<'\n';
}