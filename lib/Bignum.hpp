#ifndef BIGNUM_HPP
#define BIGNUM_HPP

#include <string>
#include <sys/types.h>
#include <vector>

using namespace std;

class Bignum {
public:
  vector<uint32_t> tab;
  bool isPositive;

  Bignum(int = 0);
  Bignum(unsigned);
  Bignum(string s);
  Bignum(Bignum const &) = default;
  Bignum(Bignum &&) = default;

  ~Bignum() = default;

  Bignum &operator=(Bignum const &) = default;
  Bignum &operator=(Bignum &&) = default;

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
  Bignum &operator<<=(unsigned n);
  Bignum &operator>>=(unsigned n);

  uint32_t &operator[](unsigned i) { return tab[i]; };
  uint32_t operator[](unsigned i) const { return tab[i]; };

  size_t size() const { return tab.size(); };
  void printHex() const;

private:
  friend bool operator<(Bignum const &, Bignum const &);
  friend bool operator==(Bignum const &, Bignum const &);
  friend bool operator<=(Bignum const &, Bignum const &);
  friend bool operator>(Bignum const &x, Bignum const &y) { return !(x <= y); };
  friend bool operator>=(Bignum const &x, Bignum const &y) { return !(x < y); };
  friend bool operator!=(Bignum const &x, Bignum const &y) {
    return !(x == y);
  };

  friend ostream &operator<<(ostream &, Bignum const &);

  friend Bignum operator<<(Bignum const &x, unsigned n) {
    auto y = x;
    return y <<= n;
  };
  friend Bignum operator>>(Bignum const &x, unsigned n) {
    auto y = x;
    return y >>= n;
  };

  friend Bignum operator+(Bignum const &, Bignum const &);
  friend Bignum operator-(Bignum const &, Bignum const &);
  friend Bignum operator*(Bignum const &, Bignum const &);
  friend Bignum operator/(Bignum const &x, Bignum const &y);
  friend Bignum operator%(Bignum const &, Bignum const &);

  friend bool compareAbs(Bignum const &x, Bignum const &y);
  friend Bignum SubtractX_Y(Bignum const &x, Bignum const &y);
  friend Bignum addSameSign(Bignum const &x, Bignum const &y);
  friend pair<Bignum, Bignum> division(Bignum, Bignum const &);

  friend Bignum modpow(Bignum, Bignum, Bignum &);

  friend bool primality_fermat(Bignum const&, uint);

  friend Bignum invMod(Bignum const &, Bignum const &);
  friend Bignum encrypt(Bignum &, Bignum &, Bignum &);
  friend Bignum decrypt(Bignum &, Bignum &, Bignum &);
};

#endif
