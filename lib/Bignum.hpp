#ifndef BIGNUM_HPP
#define BIGNUM_HPP

#include <stdint.h>

#include <string>
#include <vector>

using namespace std;

class Bignum {
public:
  vector<uint32_t> tab;
  bool isPositive;

  Bignum(int = 0);
  Bignum(unsigned int);
  Bignum(string);
  Bignum(Bignum const &) = default;
  Bignum(Bignum &&) = default;

  ~Bignum() = default;

  Bignum &operator=(Bignum const &) = default;
  Bignum &operator=(Bignum &&) = default;

  Bignum &operator>>=(unsigned n);
  Bignum &operator<<=(unsigned n);

  Bignum &operator+=(Bignum const &x) {
    *this = *this + x;
    return *this;
  };
  Bignum &operator-=(Bignum const &x) {
    *this = *this - x;
    return *this;
  };
  Bignum &operator*=(Bignum const &x) {
    *this = *this * x;
    return *this;
  };
  Bignum &operator/=(Bignum const &x) {
    *this = *this / x;
    return *this;
  };

private:
  friend bool operator<(Bignum const &, Bignum const &);
  friend bool operator<=(Bignum const &, Bignum const &);
  friend bool operator>(Bignum const &x, Bignum const &y) { return !(x <= y); };
  friend bool operator>=(Bignum const &x, Bignum const &y) { return !(x < y); };
  friend bool operator==(Bignum const &, Bignum const &);
  friend bool operator!=(Bignum const &x, Bignum const &y) {
    return !(x == y);
  };

  friend void printHex(ostream &, Bignum const &);
  friend void printDec(ostream &, Bignum const &);
  friend void printNum(ostream &, Bignum const &);

  friend ostream &operator<<(ostream &, Bignum const &);

  friend Bignum operator+(Bignum const &, Bignum const &);
  friend Bignum operator-(Bignum const &, Bignum const &);
  friend Bignum operator*(Bignum const &, Bignum const &);
  friend Bignum operator/(Bignum const &, Bignum const &);
  friend Bignum operator%(Bignum const &, Bignum const &);
  friend Bignum operator^(Bignum const &, unsigned);

  friend Bignum operator<<(Bignum const &, unsigned);
  friend Bignum operator>>(Bignum const &, unsigned);

  friend Bignum operator<<(Bignum const &x, unsigned n) {
    Bignum y = x;
    return y <<= n;
  };
  friend Bignum operator>>(Bignum const &x, unsigned n) {
    Bignum y = x;
    return y >>= n;
  };

  friend pair<Bignum, Bignum> division(Bignum const &, Bignum const &);

  friend bool compareAbs(Bignum const &x, Bignum const &y);
  friend Bignum SubtractX_Y(Bignum const &x, Bignum const &y);
  friend Bignum addSameSign(Bignum const &x, Bignum const &y);

  friend Bignum invMod(Bignum const &, Bignum const &);
  friend Bignum modPow(Bignum, Bignum, Bignum);
};

#endif