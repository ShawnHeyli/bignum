#include "Bignum.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <iomanip>  // for setw
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

void Bignum::deleteLeadingZero()
{
  while (this->tab.size() > 1 && tab.back() == 0) {
    this->tab.pop_back();
  }
}

Bignum::Bignum(const string &s) {
  string input = s;
  switch (s[0]) {
    case '+':
      input = input.erase(0, 1);
      this->isPositive = true;
      break;

    case '-':
      input = input.erase(0, 1);
      this->isPositive = false;
      break;

    default:
      this->isPositive = true;
      break;
  }

  input = divise2(input);

  int counter = 0;
  int number = 0;

  while (input[0] != ';') {
    if (input.back() == '1') number += 1 << counter;
    counter++;

    if (counter == 32) {
      this->tab.push_back(number);
      counter = 0;
      number = 0;
    }

    input = divise2(input.substr(0, input.size() - 2));
    if (input[0] == '0') {
      input = input.substr(1, input.size());
    }
  }
  if (input.back() == '1') number += 1 << counter;
  if (number > 0) this->tab.push_back(number);
  this->deleteLeadingZero();
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

// Adds a trailing zero when I do + or - not sure why :/
ostream &operator<<(ostream &stream, Bignum const &num) {
  stream << hex << uppercase << "0x" << setfill('0') << setw(8);

  for (int i = num.tab.size() - 1; i >= 0; i--) {
    stream << num.tab[i];
  }
  stream << endl;

  return stream;
}
