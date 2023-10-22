#include "../lib/Bignum.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

Bignum::Bignum(int x) {
  if (x < 0) {
    this->isPositive = false;
    this->tab.push_back(x);
  } else {
    this->isPositive = true;
    this->tab.push_back(x);
  }
}

Bignum::Bignum(unsigned num) {
  this->isPositive = true;
  this->tab.push_back(num);
}

string divise2(string const &s) {
  string resultat = "";
  int remainder = 0;
  for (char c : s) {
    if (c < '0' || c > '9') {
      throw runtime_error("invalid input");
    }
    remainder = remainder * 10 + int(c - '0');
    resultat.push_back((remainder / 2) + '0');
    remainder = remainder % 2;
  }
  resultat.append(";" + to_string(remainder));
  return resultat;
}

void Bignum::deleteLeadingZero() {
  while (this->tab.size() > 1 && this->tab.back() == 0) {
    this->tab.pop_back();
  }
}

Bignum::Bignum(std::string const &s) {
  string input = s;
  if (input.size() == 0) {
    tab = vector<uint32_t>(1, 0);
    isPositive = true;
  } else {
    if (input.at(0) == '-') {
      input.erase(0, 1);
      isPositive = false;
    } else
      isPositive = true;
    tab = vector<uint32_t>(1, 0);
    unsigned pos = 0;
    unsigned block = 0;
    while (input.size() != 0 && input.at(0) == '0') input.erase(0, 1);
    while (input.size() != 0) {
      string tmp = "";
      unsigned n = 0;
      for (auto c : input) {
        n *= 10;
        n += c - '0';
        tmp += to_string(n / 2);
        n = n % 2;
      }
      input = move(tmp);
      while (input.size() != 0 && input.at(0) == '0') input.erase(0, 1);
      tab[block] |= n << pos;

      if (pos < 31)
        pos += 1;
      else {
        pos = 0;
        block += 1;
        tab.emplace_back(0);
      }
    }
  }
}

Bignum::Bignum(Bignum const &b) {
  this->tab = b.tab;
  this->isPositive = b.isPositive;
}

Bignum::Bignum(Bignum &&b) {
  this->tab.swap(b.tab);
  this->isPositive = b.isPositive;
}

Bignum::~Bignum() {}

Bignum &Bignum::operator=(Bignum const &b) {
  this->tab = b.tab;
  this->isPositive = b.isPositive;
  return *this;
}

Bignum &Bignum::operator=(Bignum &&b) {
  this->tab.swap(b.tab);
  this->isPositive = b.isPositive;
  return *this;
}

Bignum operator+(Bignum const &x, Bignum const &y) {
  int64_t carry = 0;
  Bignum resultat = 0;
  int64_t n;
  int64_t tmp = 0;

  if (x >= y)
    n = x.tab.size();
  else
    n = y.tab.size();

  for (int i = 0; i < n; i++) {
    tmp = carry;
    if (i < x.tab.size()) tmp += x.tab[i];
    if (i < y.tab.size()) tmp += y.tab[i];

    resultat.tab.push_back(tmp % int64_t(pow(2.0, 32.0)));
    carry = tmp / pow(2, 32);
  }
  if (carry > 0) resultat.tab.push_back(carry);
  return resultat;
}

Bignum operator-(Bignum const &x, Bignum const &y) {
  Bignum resultat = 0;
  int64_t carry = 0;
  int64_t n;
  int64_t tmp = 0;

  if (x >= y)
    n = x.tab.size();
  else
    n = y.tab.size();

  for (int i = 0; i < n; i++) {
    tmp = carry;
    if (i < x.tab.size()) tmp += x.tab[i];
    if (i < y.tab.size()) tmp -= y.tab[i];

    if (tmp < 0) {
      tmp += pow(2, 32);
      carry = -1;
    } else
      carry = 0;

    resultat.tab.push_back(tmp);
  }

  return resultat;
}

Bignum operator*(Bignum const &x, Bignum const &y) {
  Bignum resultat;
  int64_t max = x.tab.size() + y.tab.size();
  int64_t carry = 0;

  for (int i = 0; i < max; i++) {
    resultat.tab.push_back(0);
  }

  for (int i = 0; i < y.tab.size(); i++) {
    carry = 0;
    for (int j = 0; j < x.tab.size(); j++) {
      int64_t tmp = resultat.tab[i + j] + x.tab[i] * y.tab[j] + carry;
      resultat.tab[i + j] = tmp % int64_t(pow(2.0, 32.0));
      carry = tmp / pow(2, 32);
    }
  }
  return resultat;
}

std::pair<Bignum, Bignum> division(Bignum const &a, Bignum const &b) {
  Bignum x;
  Bignum y;


  if (b == 0) {
    cout << "Error: Division by zero\n";
    exit(1);
  }

  // to get  n ≥ t ≥ 1
  // the bigger of the two is stored in x
  //if (a.tab > b.tab) {
  //  x = a;
  //  y = b;
  //} else {
  //  x = b;
  //  y = a;
  //}

  x=a;
  y=b;

  Bignum quotient;
  Bignum remainder;

  int n = x.tab.size() -1;
  int t = y.tab.size() -1;

  if (n == 0 && t == 0) {
    quotient.tab[0]     = x.tab[0] / y.tab[0];
    remainder.tab[0]     = x.tab[0] % y.tab[0];
    quotient.isPositive = (a.isPositive == b.isPositive);
    return make_pair(quotient, remainder);
  }

  // 1. For j from 0 to(n − t) do: q_j ← 0.
  for (int j = 0; j < (n-t); j++) {
    quotient.tab.push_back(0);
  }

  // 2. While(x ≥ y*(b^(n−t))) do the following:
  // q_(n−t)←q_(n−t) + 1,x←x − yb^(n−t).
  while (x >= (y << (32 * (n - t)))) {
    quotient.tab[n - t] += 1;
    x = x - (y << (32 * (n - t)));
  }

  // 3. For i from n down to(t + 1) do the following :
  for (int i = n; i >= (t+1); i--) {
    // 3.1 If xi = yt then set q_(i−t−1)←b − 1;
    if (x.tab[i] == y.tab[t]) {
      quotient.tab[i - t - 1] = 0xFFFFFFFF;
    } 
    // otherwise set q_(i−t−1)←(xib+xi−1)/yt).
    else { 
      quotient.tab[i - t - 1] = (((uint64_t)(x.tab[i]) << 32) + ((uint64_t)(x.tab[i-1])))/ y.tab[t];
    }

    // 3.2 While (qi−t−1(ytb + yt−1) > x_i*b^2 + x_(i−1)b + xi−2) do:
    // qi−t−1←qi−t−1 − 1.
    Bignum tmp = Bignum(quotient.tab[i - t - 1]) * Bignum((y.tab[t] << 32) + y.tab[t - 1]);
    Bignum tmp2 = Bignum(x.tab[i] << (64)) + Bignum((x.tab[i-1] << 32) + x.tab[i-2]);
    while(tmp > tmp2) {
      quotient.tab[i - t - 1] -= 1;
    }

    // 3.3 x←x − qi−t−1ybi−t−1.
    x = x - ((quotient.tab[i - t - 1]) * (y << (32 * (i - t - 1))));
    x.deleteLeadingZero();

    // 3.4 If x < 0 then set x←x + ybi−t−1 and qi−t−1←qi−t−1 − 1.
    if (x < 0) {
      x = x + (y << (32 * (i - t - 1)));
      quotient.tab[i - t - 1] -= 1;
    }
  }
  // 4. r←x.
  remainder = x;
  
  quotient.isPositive = (a.isPositive == b.isPositive);
  remainder.isPositive = a.isPositive;

  remainder.deleteLeadingZero();
  quotient.deleteLeadingZero();

  return make_pair(quotient, remainder);
}

Bignum operator/(Bignum const &x, Bignum const &y) {
  return division(x, y).first;
}

Bignum operator%(Bignum const &x, Bignum const &y) {
  return division(x, y).second;
}

// Exponentiation rapide modulaire
Bignum operator^(Bignum const &e, unsigned m) {
  Bignum resultat = 1;
  Bignum a = e;
  while (m > 0) {
    if (m % 2 == 1) {
      resultat = resultat * a;
    }
    a = a * a;
    m = m / 2;
  }
  return resultat;
}

Bignum operator<<(Bignum const &x, unsigned n) {
  Bignum resultat = x;
  if (n == 0) return resultat;
  unsigned n_bits = n % 32;
  unsigned n_blocks = n / 32;
  if (n_bits == 0) {
    resultat.tab.resize(resultat.tab.size() + n_blocks);
    for (unsigned i = resultat.tab.size(); i-- != n_blocks;)
      resultat.tab[i] = resultat.tab[i - n_blocks];
  } else {
    resultat.tab.resize(resultat.tab.size() + n_blocks + 1);
    for (unsigned i = resultat.tab.size(); i-- != n_blocks + 1;)
      resultat.tab[i] = (resultat.tab[i - n_blocks] << n_bits) |
                        (resultat.tab[i - (n_blocks + 1)] >> (32 - n_bits));
    resultat.tab[n_blocks] = resultat.tab[0] << n_bits;
  }
  for (unsigned i = 0; i < n_blocks; ++i) resultat.tab[i] = 0u;

  unsigned ntab = resultat.tab.size();
  while (ntab > 0 && resultat.tab[ntab - 1] == 0) --ntab;
  resultat.tab.resize(ntab);

  return resultat;
}

bool operator<(Bignum const &x, Bignum const &y) {
  if (x.tab.size() < y.tab.size()) {
    return true;
  } else if (x.tab.size() > y.tab.size()) {
    return false;
  }
  for (int i = x.tab.size(); i >= 0; i--) {
    if (x.tab[i] < y.tab[i]) {
      return true;
    } else if (x.tab[i] > y.tab[i]) {
      return false;
    }
  }
  return false;
}

bool operator<=(Bignum const &x, Bignum const &y) { return (x < y || x == y); }

bool operator>(Bignum const &x, Bignum const &y) { return !(x <= y); }

bool operator>=(Bignum const &x, Bignum const &y) { return !(x < y); }

bool operator==(Bignum const &x, Bignum const &y) {
  if (x.isPositive != y.isPositive) return false;
  if (x.tab.size() != y.tab.size()) return false;

  for (int i = 0; i < x.tab.size(); i++) {
    if (x.tab[i] != y.tab[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const Bignum &x, const Bignum &y) { return !(x == y); }

void printHex(ostream &stream, Bignum const &num) {
  if (!num.isPositive) {
    stream << "-";
  }
  stream << hex << uppercase << "0x" << setfill('0') << setw(8);

  for (int i = num.tab.size() - 1; i >= 0; i--) {
    stream << num.tab[i];
  }
  stream << endl;
}

void printDec(ostream &stream, Bignum const &num) {
  if (!num.isPositive){
    stream << "-1 * ";
  }

  stream << '(' << num.tab[0];

  for (unsigned int i = 1; i < num.tab.size() - 1; i++){
    unsigned long tmp = num.tab[i] * pow(i, 32);
    stream << " + " << tmp ;

  }
  stream << ')' << endl;
}

// Adds a trailing zero when I do + or - not sure why :/
ostream &operator<<(ostream &stream, Bignum const &num) {
  printDec(stream, num);
  return stream;
}