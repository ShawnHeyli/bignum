#include "../lib/Bignum.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

#include "../lib/Bignum.hpp"

using namespace std;

// Donnée sur la branche main
Bignum::Bignum(int x) : tab(1, x >= 0 ? x : -x), isPositive(x >= 0) {}

// Donnée sur la branche main
Bignum::Bignum(unsigned x) : tab(1, x), isPositive(true) {}

// Donnée sur la branche main
Bignum::Bignum(string s)
{
  if (s.size() == 0)
  {
    tab = vector<uint32_t>(1, 0);
    isPositive = true;
  }
  else
  {
    if (s.at(0) == '-')
    {
      s.erase(0, 1);
      isPositive = false;
    }
    else
      isPositive = true;
    tab = vector<uint32_t>(1, 0);
    unsigned pos = 0;
    unsigned block = 0;
    while (s.size() != 0 && s.at(0) == '0')
      s.erase(0, 1);
    while (s.size() != 0)
    {
      string tmp = "";
      unsigned n = 0;
      for (auto c : s)
      {
        n *= 10;
        n += c - '0';
        tmp += to_string(n / 2);
        n = n % 2;
      }
      s = std::move(tmp);
      while (s.size() != 0 && s.at(0) == '0')
        s.erase(0, 1);
      tab[block] |= n << pos;
      if (pos < 31)
        pos += 1;
      else
      {
        pos = 0;
        block += 1;
        tab.emplace_back(0);
      }
    }
  }
}

Bignum addSameSign(Bignum const &num, Bignum const &y)
{
  if (y.tab.size() > num.tab.size())
    return addSameSign(y, num);
  Bignum z(0u);
  z.tab.resize(num.tab.size());
  uint64_t c = 0;
  for (unsigned i = 0; i < y.tab.size(); ++i)
  {
    uint64_t tmp = num.tab[i] + (y.tab[i] + c);
    z.tab[i] = tmp;
    c = tmp >> 32;
  }
  for (unsigned i = y.tab.size(); i < num.tab.size(); ++i)
  {
    uint64_t tmp = num.tab[i] + c;
    z.tab[i] = tmp;
    c = tmp >> 32;
  }
  if (c != 0)
    z.tab.emplace_back(1);
  return z;
}

Bignum SubtractX_Y(Bignum const &num, Bignum const &y)
{
  // assume num >= y >= 0
  Bignum z(0u);
  z.tab.resize(num.tab.size());
  uint64_t c = 0;
  for (unsigned i = 0; i < y.tab.size(); ++i)
  {
    uint64_t tmp = num.tab[i] - (y.tab[i] + c);
    z.tab[i] = tmp;
    if ((tmp >> 32) != 0)
      c = 1;
    else
      c = 0;
  }
  for (unsigned i = y.tab.size(); i < num.tab.size(); ++i)
  {
    uint64_t tmp = num.tab[i] - c;
    z.tab[i] = tmp;
    if ((tmp >> 32) != 0)
      c = 1;
    else
      c = 0;
  }
  return z;
}

bool compareAbs(Bignum const &num, Bignum const &y)
{
  unsigned nx = num.tab.size();
  while (nx > 0 && num.tab[nx - 1] == 0)
    --nx;
  unsigned ny = y.tab.size();
  while (ny > 0 && y.tab[ny - 1] == 0)
    --ny;
  if (nx != ny)
    return nx > ny;
  while (nx > 0)
  {
    --nx;
    if (num.tab[nx] != y.tab[nx])
      return num.tab[nx] > y.tab[nx];
  }
  return true;
}

Bignum operator+(Bignum const &num, Bignum const &y)
{
  if (num.isPositive == y.isPositive)
  {
    auto z = addSameSign(num, y);
    z.isPositive = num.isPositive;
    return z;
  }
  else
  {
    if (num.isPositive)
    {
      if (compareAbs(num, y))
        return SubtractX_Y(num, y);
      auto z = SubtractX_Y(y, num);
      z.isPositive = false;
      return z;
    }
    else
    {
      if (compareAbs(y, num))
        return SubtractX_Y(y, num);
      auto z = SubtractX_Y(num, y);
      z.isPositive = false;
      return z;
    }
  }
}

Bignum operator-(Bignum const &num, Bignum const &y)
{
  if (num.isPositive == y.isPositive)
  {
    if (compareAbs(num, y))
    {
      auto z = SubtractX_Y(num, y);
      z.isPositive = num.isPositive;
      return z;
    }
    else
    {
      auto z = SubtractX_Y(y, num);
      z.isPositive = !num.isPositive;
      return z;
    }
  }
  else
  {
    auto z = addSameSign(num, y);
    z.isPositive = num.isPositive;
    return z;
  }
}

Bignum operator*(Bignum const &num, Bignum const &y)
{
  Bignum w(0);
  unsigned n = num.tab.size() - 1, t = y.tab.size() - 1;
  w.tab.resize(n + t + 2);
  for (unsigned i = 0; i <= t; ++i)
  {
    uint64_t c = 0;
    for (unsigned j = 0; j <= n; ++j)
    {
      uint64_t tmp = y.tab[i];
      tmp *= num.tab[j];
      tmp += c;
      tmp += w.tab[i + j];
      c = tmp >> 32;
      w.tab[i + j] = tmp;
    }
    w.tab[i + n + 1] = c;
  }
  w.isPositive = (num.isPositive == y.isPositive);
  return w;
}

pair<Bignum, Bignum> division(Bignum const &X, Bignum const &Y)
{

  Bignum num = X;
  Bignum y = Y;

  // num et y >= 0
  if (num < y)
    return make_pair(Bignum(0), num);
  unsigned t = y.tab.size() - 1;
  while (t >= 0 && y.tab[t] == 0)
    --t;
  unsigned n = 0;
  while ((y.tab[t] << n) < (1u << 31))
    ++n;
  if (n != 0 || t == 0)
  {
    if (t == 0)
      n += 32;
    auto p = division(num << n, y << n);
    p.second = p.second >> n;
    return p;
  }
  n = num.tab.size() - 1;
  while (n >= 0 && num.tab[n] == 0)
    --n;
  Bignum q(0);
  q.tab.resize(n - t + 1);
  while (num >= (y << 32 * (n - t)))
  {
    q.tab[n - t] += 1;
    num = num - (y << 32 * (n - t));
  }
  for (unsigned i = n; i >= t + 1; --i)
  {
    if (num.tab[i] == y.tab[t])
      q.tab[i - t - 1] = 0xFFFFFFFF;
    else
    {
      uint64_t tmp = num.tab[i];
      tmp <<= 32;
      tmp += num.tab[i - 1];
      q.tab[i - t - 1] = tmp / y.tab[t];
    }
    Bignum a(y.tab[t]);
    a <<= 32;
    a += Bignum(y.tab[t - 1]);
    Bignum b(num.tab[i]);
    b <<= 32;
    b += Bignum(num.tab[i - 1]);
    b <<= 32;
    b += Bignum(num.tab[i - 2]);
    while (Bignum(q.tab[i - t - 1]) * a > b)
    {
      q.tab[i - t - 1] -= 1;
    }
    num -= (Bignum(q.tab[i - t - 1]) * y) << (32 * (i - t - 1));
    if (num < 0)
    {
      num += y << (32 * (i - t - 1));
      q.tab[i - t - 1] -= 1;
    }
  }
  while (q.tab.size() > 1 and q.tab.back() == 0)
  {
    q.tab.pop_back();
  }

  return make_pair(q, num);
}

Bignum operator/(Bignum const &num, Bignum const &y)
{
  auto xx = num; xx.isPositive = true;
  auto yy = y; yy.isPositive = true;
  auto p = division(std::move(xx),std::move(yy));
  if (num.isPositive != y.isPositive) p.first.isPositive = false;
  return p.first;
}

Bignum operator%(Bignum const &num, Bignum const &y)
{
  auto xx = num; xx.isPositive = true;
  auto yy = y; yy.isPositive = true;
  auto p = division(std::move(xx),std::move(yy));
  if (num.isPositive == y.isPositive && !num.isPositive) p.second.isPositive = false;
  return p.second;
}

Bignum & Bignum::operator<<=(unsigned n) {
  if (n == 0) return *this;
  unsigned n_bits = n%32;
  unsigned n_blocks = n/32;
  if (n_bits == 0) {
    tab.resize(tab.size() + n_blocks);
    for (unsigned i = tab.size(); i-- != n_blocks; ) tab[i] = tab[i-n_blocks];
  }
  else {
    tab.resize(tab.size() + n_blocks + 1);
    for (unsigned i = tab.size(); i-- != n_blocks+1; ) tab[i] = (tab[i-n_blocks] << n_bits) | (tab[i - (n_blocks + 1)] >> (32-n_bits));
    tab[n_blocks] = tab[0] << n_bits;
  }
  for (unsigned i = 0; i < n_blocks; ++i) tab[i] = 0u;

  unsigned ntab = tab.size();
  while (ntab > 1 && tab[ntab-1] == 0) --ntab;
  tab.resize(ntab);

  return *this;
}

Bignum & Bignum::operator>>=(unsigned n) {
  if (n == 0) return *this;
  unsigned n_bits = n%32;
  unsigned n_blocks = n/32;
  if (n_bits == 0) {
    for (unsigned i = 0; i + n_blocks < tab.size(); ++i) tab[i] = tab[i+n_blocks];
    if (tab.size() > n_blocks) tab.resize(tab.size() - n_blocks);
    else {
      tab.resize(1);
      tab[0] = 0;
    }
  }
  else {

    for (unsigned i = 0; i + 1 +  n_blocks < tab.size(); ++i) tab[i] = (tab[i+n_blocks] >> n_bits) | (tab[i + (n_blocks + 1)] << (32-n_bits));
    tab[tab.size() - n_blocks - 1] = tab[tab.size() - 1] >> n_bits;
  }

  unsigned ntab = tab.size();
  while (ntab > 1 && tab[ntab-1] == 0) --ntab;
  tab.resize(ntab);

  return *this;
}

bool operator<(Bignum const & num, Bignum const & y) {
    if (num.isPositive != y.isPositive) return y.isPositive;
    unsigned n = num.tab.size()-1, t = y.tab.size()-1;
    while (n > 0 && num.tab[n] == 0) --n;
    while (t > 0 && y.tab[t] == 0) --t;
    if (n != t) return num.isPositive ^ (n > t);
    while (n != 0) {
      if (num.tab[n] != y.tab[n]) return num.isPositive ^ (num.tab[n] > y.tab[n]);
      --n;
    }
    if (num.tab[0] == y.tab[0]) return false;
    return num.isPositive ^ (num.tab[0] > y.tab[0]);
}

bool operator<=(Bignum const & num, Bignum const & y) {
    if (num.isPositive != y.isPositive) return y.isPositive;
    unsigned n = num.tab.size()-1, t = y.tab.size()-1;
    while (n > 0 && num.tab[n] == 0) --n;
    while (t > 0 && y.tab[t] == 0) --t;
    if (n != t) return num.isPositive ^ (n > t);
    while (n != 0) {
      if (num.tab[n] != y.tab[n]) return num.isPositive ^ (num.tab[n] > y.tab[n]);
      --n;
    }
    if (num.tab[0] == y.tab[0]) return true;
    return num.isPositive ^ (num.tab[0] > y.tab[0]);
}

bool operator==(Bignum const & num, Bignum const & y) {
    unsigned n = num.tab.size()-1, t = y.tab.size()-1;
    while (n > 0 && num.tab[n] == 0) --n;
    while (t > 0 && y.tab[t] == 0) --t;
    if (n != t) return false;
    if (n != 0 && num.isPositive != y.isPositive) return false;
    while (n != 0) {
      if (num.tab[n] != y.tab[n]) return false;
      --n;
    }
    return (num.tab[0] == y.tab[0]) && (num.isPositive == y.isPositive || num.tab[0] == 0);
}

Bignum modPow(Bignum base, Bignum exp, Bignum mod)
{
  if (base < 0 || exp < 0 || mod < 0)
  {
    cerr << "Error modpow, negative value" << endl;
    exit(1);
  }
  if (mod == 1)
  {
    return Bignum(0);
  }
  if (base == 1)
  {
    return Bignum(1);
  }
  Bignum result(1);
  base = base % mod;
  while (exp > 0)
  {
    if (exp.tab[0] & 1)
    {
      result = (result * base) % mod;
    }
    exp = exp >> 1;
    base = (base * base) % mod;
  }

  return result;
}

Bignum invMod(Bignum const &a, Bignum const &b)
{
  Bignum u = 1;
  Bignum u_ = 0;
  Bignum r = a;
  Bignum r_ = b;

  while (r_ != 0)
  {
    Bignum q = r / r_;
    Bignum rs = r;
    Bignum us = u;
    r = r_;
    u = u_;

    r_ = rs - (q * r);
    u_ = us - (q * u);
  }

  if (u < Bignum(0))
  {
    u = u + b;
  }

  return u;
}

Bignum operator^(Bignum const &e, unsigned m)
{
  Bignum resultat = 1;
  Bignum tmp = e;
  while (m > 0)
  {
    if (m % 2 == 1)
      resultat = resultat * tmp;
    tmp = tmp * tmp;
    m /= 2;
  }
  return resultat;
}

ostream &operator<<(ostream &stream, Bignum const &num)
{
  if (!num.isPositive) cout << "-";
  stream << "0x";
  bool first = true;
  for (auto rit = num.tab.rbegin(); rit != num.tab.rend() ; ++rit) {
    if (first && *rit == 0) continue;
    stream << hex;
    if (!first) cout << setfill('0') << setw(8);
    else first = false;
    stream << *rit << dec;
  }
  if (first) stream << 0;
  return stream;
}