#include "../lib/Bignum.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include "../lib/Bignum.hpp"

using namespace std;

Bignum::Bignum(int x)
{
  if (x < 0)
  {
    this->isPositive = false;
    this->tab.push_back(x);
  }
  else
  {
    this->isPositive = true;
    this->tab.push_back(x);
  }
}

Bignum::Bignum(unsigned num)
{
  this->isPositive = true;
  this->tab.push_back(num);
}

string divise2(string const &s)
{
  string resultat = "";
  int remainder = 0;
  for (char c : s)
  {
    if (c < '0' || c > '9')
    {
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
  while (this->tab.size() > 1 && this->tab.back() == 0)
  {
    this->tab.pop_back();
  }
}

Bignum::Bignum(std::string const &s)
{
  string input = s;
  if (input.size() == 0)
  {
    tab = vector<uint32_t>(1, 0);
    isPositive = true;
  }
  else
  {
    if (input.at(0) == '-')
    {
      input.erase(0, 1);
      isPositive = false;
    }
    else
      isPositive = true;
    tab = vector<uint32_t>(1, 0);
    unsigned pos = 0;
    unsigned block = 0;
    while (input.size() != 0 && input.at(0) == '0')
      input.erase(0, 1);
    while (input.size() != 0)
    {
      string tmp = "";
      unsigned n = 0;
      for (auto c : input)
      {
        n *= 10;
        n += c - '0';
        tmp += to_string(n / 2);
        n = n % 2;
      }
      input = move(tmp);
      while (input.size() != 0 && input.at(0) == '0')
        input.erase(0, 1);
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

Bignum addSameSign(Bignum const &x, Bignum const &y)
{
  if (y.tab.size() > x.tab.size())
    return addSameSign(y, x);
  Bignum z(0u);
  z.tab.resize(x.tab.size());
  uint64_t c = 0;
  for (unsigned i = 0; i < y.tab.size(); ++i)
  {
    uint64_t tmp = x.tab[i] + (y.tab[i] + c);
    z.tab[i] = tmp;
    c = tmp >> 32;
  }
  for (unsigned i = y.tab.size(); i < x.tab.size(); ++i)
  {
    uint64_t tmp = x.tab[i] + c;
    z.tab[i] = tmp;
    c = tmp >> 32;
  }
  if (c != 0)
    z.tab.emplace_back(1);
  return z;
}

Bignum SubtractX_Y(Bignum const &x, Bignum const &y)
{
  // assume x >= y >= 0
  Bignum z(0u);
  z.tab.resize(x.tab.size());
  uint64_t c = 0;
  for (unsigned i = 0; i < y.tab.size(); ++i)
  {
    uint64_t tmp = x.tab[i] - (y.tab[i] + c);
    z.tab[i] = tmp;
    if ((tmp >> 32) != 0)
      c = 1;
    else
      c = 0;
  }
  for (unsigned i = y.tab.size(); i < x.tab.size(); ++i)
  {
    uint64_t tmp = x.tab[i] - c;
    z.tab[i] = tmp;
    if ((tmp >> 32) != 0)
      c = 1;
    else
      c = 0;
  }
  return z;
}

bool compareAbs(Bignum const &x, Bignum const &y)
{
  unsigned nx = x.tab.size();
  while (nx > 0 && x.tab[nx - 1] == 0)
    --nx;
  unsigned ny = y.tab.size();
  while (ny > 0 && y.tab[ny - 1] == 0)
    --ny;
  if (nx != ny)
    return nx > ny;
  while (nx > 0)
  {
    --nx;
    if (x.tab[nx] != y.tab[nx])
      return x.tab[nx] > y.tab[nx];
  }
  return true;
}

Bignum operator+(Bignum const &x, Bignum const &y)
{
  if (x.isPositive == y.isPositive)
  {
    auto z = addSameSign(x, y);
    z.isPositive = x.isPositive;
    return z;
  }
  else
  {
    if (x.isPositive)
    {
      if (compareAbs(x, y))
        return SubtractX_Y(x, y);
      auto z = SubtractX_Y(y, x);
      z.isPositive = false;
      return z;
    }
    else
    {
      if (compareAbs(y, x))
        return SubtractX_Y(y, x);
      auto z = SubtractX_Y(x, y);
      z.isPositive = false;
      return z;
    }
  }
}

Bignum operator-(Bignum const &x, Bignum const &y)
{
  if (x.isPositive == y.isPositive)
  {
    if (compareAbs(x, y))
    {
      auto z = SubtractX_Y(x, y);
      z.isPositive = x.isPositive;
      return z;
    }
    else
    {
      auto z = SubtractX_Y(y, x);
      z.isPositive = !x.isPositive;
      return z;
    }
  }
  else
  {
    auto z = addSameSign(x, y);
    z.isPositive = x.isPositive;
    return z;
  }
}

Bignum operator*(Bignum const &x, Bignum const &y)
{
  Bignum w(0);
  unsigned n = x.tab.size() - 1, t = y.tab.size() - 1;
  w.tab.resize(n + t + 2);
  for (unsigned i = 0; i <= t; ++i)
  {
    uint64_t c = 0;
    for (unsigned j = 0; j <= n; ++j)
    {
      uint64_t tmp = y.tab[i];
      tmp *= x.tab[j];
      tmp += c;
      tmp += w.tab[i + j];
      c = tmp >> 32;
      w.tab[i + j] = tmp;
    }
    w.tab[i + n + 1] = c;
  }
  w.isPositive = (x.isPositive == y.isPositive);
  return w;
}

pair<Bignum, Bignum> division(Bignum const &X, Bignum const &Y)
{

  Bignum x = X;
  Bignum y = Y;

  // x et y >= 0
  if (x < y)
    return make_pair(Bignum(0), x);
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
    auto p = division(x << n, y << n);
    p.second = p.second >> n;
    return p;
  }
  n = x.tab.size() - 1;
  while (n >= 0 && x.tab[n] == 0)
    --n;
  Bignum q(0);
  q.tab.resize(n - t + 1);
  while (x >= (y << 32 * (n - t)))
  {
    q.tab[n - t] += 1;
    x = x - (y << 32 * (n - t));
  }
  for (unsigned i = n; i >= t + 1; --i)
  {
    if (x.tab[i] == y.tab[t])
      q.tab[i - t - 1] = 0xFFFFFFFF;
    else
    {
      uint64_t tmp = x.tab[i];
      tmp <<= 32;
      tmp += x.tab[i - 1];
      q.tab[i - t - 1] = tmp / y.tab[t];
    }
    Bignum a(y.tab[t]);
    a <<= 32;
    a += Bignum(y.tab[t - 1]);
    Bignum b(x.tab[i]);
    b <<= 32;
    b += Bignum(x.tab[i - 1]);
    b <<= 32;
    b += Bignum(x.tab[i - 2]);
    while (Bignum(q.tab[i - t - 1]) * a > b)
    {
      q.tab[i - t - 1] -= 1;
    }
    x -= (Bignum(q.tab[i - t - 1]) * y) << (32 * (i - t - 1));
    if (x < 0)
    {
      x += y << (32 * (i - t - 1));
      q.tab[i - t - 1] -= 1;
    }
  }
  while (q.tab.size() > 1 and q.tab.back() == 0)
  {
    q.tab.pop_back();
  }

  return make_pair(q, x);
}

Bignum operator/(Bignum const &x, Bignum const &y)
{
  auto xx = x; xx.isPositive = true;
  auto yy = y; yy.isPositive = true;
  auto p = division(move(xx),move(yy));
  if (x.isPositive != y.isPositive) p.first.isPositive = false;
  return p.first;
}

Bignum operator%(Bignum const &x, Bignum const &y)
{
  auto xx = x; xx.isPositive = true;
  auto yy = y; yy.isPositive = true;
  auto p = division(move(xx),move(yy));
  if (x.isPositive == y.isPositive && !x.isPositive) p.second.isPositive = false;
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

bool operator<(Bignum const & x, Bignum const & y) {
    if (x.isPositive != y.isPositive) return y.isPositive;
    unsigned n = x.tab.size()-1, t = y.tab.size()-1;
    while (n > 0 && x.tab[n] == 0) --n;
    while (t > 0 && y.tab[t] == 0) --t;
    if (n != t) return x.isPositive ^ (n > t);
    while (n != 0) {
      if (x.tab[n] != y.tab[n]) return x.isPositive ^ (x.tab[n] > y.tab[n]);
      --n;
    }
    if (x.tab[0] == y.tab[0]) return false;
    return x.isPositive ^ (x.tab[0] > y.tab[0]);
}

bool operator<=(Bignum const & x, Bignum const & y) {
    if (x.isPositive != y.isPositive) return y.isPositive;
    unsigned n = x.tab.size()-1, t = y.tab.size()-1;
    while (n > 0 && x.tab[n] == 0) --n;
    while (t > 0 && y.tab[t] == 0) --t;
    if (n != t) return x.isPositive ^ (n > t);
    while (n != 0) {
      if (x.tab[n] != y.tab[n]) return x.isPositive ^ (x.tab[n] > y.tab[n]);
      --n;
    }
    if (x.tab[0] == y.tab[0]) return true;
    return x.isPositive ^ (x.tab[0] > y.tab[0]);
}

bool operator==(Bignum const & x, Bignum const & y) {
    unsigned n = x.tab.size()-1, t = y.tab.size()-1;
    while (n > 0 && x.tab[n] == 0) --n;
    while (t > 0 && y.tab[t] == 0) --t;
    if (n != t) return false;
    if (n != 0 && x.isPositive != y.isPositive) return false;
    while (n != 0) {
      if (x.tab[n] != y.tab[n]) return false;
      --n;
    }
    return (x.tab[0] == y.tab[0]) && (x.isPositive == y.isPositive || x.tab[0] == 0);
}

Bignum modPow(Bignum base, Bignum exp, Bignum m)
{

  if (base < 0 || exp < 0 || m < 0)
  {
    cerr << "Error modpow, negative value" << endl;
    exit(1);
  }
  if (m == 1)
  {
    return Bignum(0);
  }
  if (base == 1)
  {
    return Bignum(1);
  }
  Bignum result(1);
  base = base % m;
  while (exp > 0)
  {
    if (exp.tab[0] & 1)
    {
      result = (result * base) % m;
    }
    exp = exp >> 1;
    base = (base * base) % m;
  }
  result.deleteLeadingZero();
  return result;
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

void printHex(ostream &stream, Bignum const &num)
{
  if (!num.isPositive)
  {
    stream << "-";
  }
  stream << hex << uppercase << "0x" << setfill('0') << setw(8);

  for (int i = num.tab.size() - 1; i >= 0; i--)
  {
    stream << num.tab[i];
  }
}

void printDec(ostream &stream, Bignum const &num)
{
  if (!num.isPositive)
  {
    stream << "-1 * ";
  }

  stream << '(' << num.tab[0];

  for (unsigned int i = 1; i < num.tab.size() - 1; i++)
  {
    unsigned long tmp = num.tab[i] * pow(i, 32);
    stream << " + " << tmp;
  }
  stream << ')';
}

ostream &operator<<(ostream &stream, Bignum const &num)
{
  printDec(stream, num);
  return stream;
}