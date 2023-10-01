#ifndef BIGNUM_HPP
#define BIGNUM_HPP

#include <stdint.h>
#include <string>
#include <vector>

class Bignum
{
public:
  Bignum(int = 0);
  Bignum(unsigned);
  void deleteLeadingZero();
  Bignum(std::string const &s);
  Bignum(Bignum const &);
  Bignum(Bignum &&);

  ~Bignum();

  Bignum &operator=(Bignum const &);
  Bignum &operator=(Bignum &&);

  Bignum &operator+=(Bignum const &);
  Bignum &operator-=(Bignum const &);
  Bignum &operator*=(Bignum const &);
  Bignum &operator/=(Bignum const &);

private:
  std::vector<uint32_t> tab;
  bool isPositive;

  friend bool operator<(Bignum const &, Bignum const &);
  friend bool operator<=(Bignum const &, Bignum const &);
  friend bool operator>(Bignum const &, Bignum const &);
  friend bool operator>=(Bignum const &, Bignum const &);
  friend bool operator==(Bignum const &, Bignum const &);
  friend bool operator!=(Bignum const &, Bignum const &);

  friend std::ostream &operator<<(std::ostream &, Bignum const &);

  friend Bignum operator+(Bignum const &, Bignum const &);
  friend Bignum operator-(Bignum const &, Bignum const &);
  friend Bignum operator*(Bignum const &, Bignum const &);
  friend Bignum operator/(Bignum const &, Bignum const &);
  friend Bignum operator%(Bignum const &, Bignum const &);

  friend std::pair<Bignum, Bignum> division(Bignum const &, Bignum const &);

  friend Bignum inverseMod(Bignum const &, Bignum const &);
};

#endif