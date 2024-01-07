/// @author Hilan Meyran

#include "../lib/Bignum.hpp"
#include <iomanip>
#include <iostream>

using namespace std;

/**
 * @brief Construit un Bignum a partir d'un entier
 *
 * Le constructeur initialise un Bignum a partir d'un entier.
 * L'entier peut etre positif ou negatif.
 *
 * @param x L'entier a convertir en Bignum
 */
Bignum::Bignum(int x) : tab(1, x >= 0 ? x : -x), isPositive(x >= 0) {}

/**
 * @brief Construit un Bignum a partir d'un entier non signé
 *
 * Construit un Bignum positif a partir d'un entier non signé.
 *
 * @param x L'entier non signé a convertir en Bignum
 */
Bignum::Bignum(unsigned x) : tab(1, x), isPositive(true) {}

/**
 * @brief Construit un Bignum a partir d'une chaine de caractere
 *
 * Le string peut representer un nombre positif ou negatif. Si le string est
 * vide, le Bignum est initialise a zero. Si le string commence par un signe
 * '-', le Bignum est initialise a un nombre negatif. Sinon, il est initialise a
 * un nombre positif.
 *
 * Le string est converti en representation binaire en utilisant l'arithmetique
 * en base 10. Chaque chiffre du string est converti en sa representation
 * binaire et stocke dans le vecteur interne du Bignum. La representation
 * binaire est stockee en blocs de 32 bits, le bit de poids faible etant stocké
 * dans le premier bloc.
 *
 * @param s La representation en chaine de caractere du nombre
 */
Bignum::Bignum(string s) {
  if (s.size() == 0) {
    tab = vector<uint32_t>(1, 0);
    isPositive = true;
  } else {
    if (s.at(0) == '-') {
      s.erase(0, 1);
      isPositive = false;
    } else
      isPositive = true;
    tab = vector<uint32_t>(1, 0);
    unsigned pos = 0;
    unsigned block = 0;
    while (s.size() != 0 && s.at(0) == '0')
      s.erase(0, 1);
    while (s.size() != 0) {
      string tmp = "";
      unsigned n = 0;
      for (auto c : s) {
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
      else {
        pos = 0;
        block += 1;
        tab.emplace_back(0);
      }
    }
  }
}

/**
 * @brief Additionnes deux bignum de meme signe
 *
 * @return Resultat de l'addition
 */
Bignum addSameSign(Bignum const &x, Bignum const &y) {
  if (y.tab.size() > x.tab.size())
    return addSameSign(y, x);
  Bignum z(0u);
  z.tab.resize(x.tab.size());
  uint64_t c = 0;
  for (unsigned i = 0; i < y.tab.size(); ++i) {
    uint64_t tmp = x.tab[i] + (y.tab[i] + c);
    z.tab[i] = tmp;
    c = tmp >> 32;
  }
  for (unsigned i = y.tab.size(); i < x.tab.size(); ++i) {
    uint64_t tmp = x.tab[i] + c;
    z.tab[i] = tmp;
    c = tmp >> 32;
  }
  if (c != 0)
    z.tab.emplace_back(1);
  return z;
}

/**
 * Soustrait deux bignum
 *
 * @return Resultat de la soustraction
 */
Bignum SubtractX_Y(Bignum const &x, Bignum const &y) {
  // assume x >= y >= 0
  Bignum z(0u);
  z.tab.resize(x.tab.size());
  uint64_t c = 0;
  for (unsigned i = 0; i < y.tab.size(); ++i) {
    uint64_t tmp = x.tab[i] - (y.tab[i] + c);
    z.tab[i] = tmp;
    if ((tmp >> 32) != 0)
      c = 1;
    else
      c = 0;
  }
  for (unsigned i = y.tab.size(); i < x.tab.size(); ++i) {
    uint64_t tmp = x.tab[i] - c;
    z.tab[i] = tmp;
    if ((tmp >> 32) != 0)
      c = 1;
    else
      c = 0;
  }
  return z;
}

/**
 * @brief Compare la valeur absolue de deux bignum
 *
 * @return true if the absolute value of x is greater than the absolute value of
 * y.
 * @return false if the absolute value of x is less than or equal to the
 * absolute value of y.
 */
bool compareAbs(Bignum const &x, Bignum const &y) {
  unsigned nx = x.tab.size();
  while (nx > 0 && x.tab[nx - 1] == 0)
    --nx;
  unsigned ny = y.tab.size();
  while (ny > 0 && y.tab[ny - 1] == 0)
    --ny;
  if (nx != ny)
    return nx > ny;
  while (nx > 0) {
    --nx;
    if (x.tab[nx] != y.tab[nx])
      return x.tab[nx] > y.tab[nx];
  }
  return true;
}

/**
 * @brief Additionnes deux bignum
 * @return Resultat de l'addition
 */
Bignum operator+(Bignum const &x, Bignum const &y) {
  if (x.isPositive == y.isPositive) {
    auto z = addSameSign(x, y);
    z.isPositive = x.isPositive;
    return z;
  } else {
    if (x.isPositive) {
      if (compareAbs(x, y))
        return SubtractX_Y(x, y);
      auto z = SubtractX_Y(y, x);
      z.isPositive = false;
      return z;
    } else {
      if (compareAbs(y, x))
        return SubtractX_Y(y, x);
      auto z = SubtractX_Y(x, y);
      z.isPositive = false;
      return z;
    }
  }
}

/**
 * @brief Soustrait deux bignum
 * @return Resultat de la soustraction
 */
Bignum operator-(Bignum const &x, Bignum const &y) {
  if (x.isPositive == y.isPositive) {
    if (compareAbs(x, y)) {
      auto z = SubtractX_Y(x, y);
      z.isPositive = x.isPositive;
      return z;
    } else {
      auto z = SubtractX_Y(y, x);
      z.isPositive = !x.isPositive;
      return z;
    }
  } else {
    auto z = addSameSign(x, y);
    z.isPositive = x.isPositive;
    return z;
  }
}

/**
 * @brief Multiplie deux bignum
 * @return Resultat de la multiplication
 */
Bignum operator*(Bignum const &x, Bignum const &y) {
  Bignum w(0);
  unsigned n = x.size() - 1, t = y.size() - 1;
  w.tab.resize(n + t + 2);
  for (unsigned i = 0; i <= t; ++i) {
    uint64_t c = 0;
    for (unsigned j = 0; j <= n; ++j) {
      uint64_t tmp = y[i];
      tmp *= x[j];
      tmp += c;
      tmp += w[i + j];
      c = tmp >> 32;
      w[i + j] = tmp;
    }
    w[i + n + 1] = c;
  }
  while (w.tab.size() > 1 && w.tab[w.tab.size() - 1] == 0) {
    w.tab.pop_back();
  }
  w.isPositive = (x.isPositive == y.isPositive);
  return w;
}

/**
 * @brief Divise x par y
 *
 * @param x numérateur
 * @param y denominateur
 * @return le quotient et le reste de la division
 */
pair<Bignum, Bignum> division(Bignum x, Bignum const &y) {
  // x et y >= 0
  if (x < y)
    return make_pair(Bignum(0), x);
  unsigned t = y.size() - 1;
  while (t >= 0 && y.tab[t] == 0)
    --t;
  unsigned n = 0;
  while ((y.tab[t] << n) < (1u << 31))
    ++n;
  if (n != 0 || t == 0) {
    if (t == 0)
      n += 32;
    auto p = division(x << n, y << n);
    p.second = p.second >> n;
    return p;
  }
  n = x.size() - 1;
  while (n >= 0 && x.tab[n] == 0)
    --n;
  Bignum q(0);
  q.tab.resize(n - t + 1);
  while (x >= (y << 32 * (n - t))) {
    q.tab[n - t] += 1;
    x = x - (y << 32 * (n - t));
  }
  for (unsigned i = n; i >= t + 1; --i) {
    if (x.tab[i] == y.tab[t])
      q.tab[i - t - 1] = 0xFFFFFFFF;
    else {
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
    while (Bignum(q.tab[i - t - 1]) * a > b) {
      q.tab[i - t - 1] -= 1;
    }
    x -= (Bignum(q.tab[i - t - 1]) * y) << (32 * (i - t - 1));
    if (x < 0) {
      x += y << (32 * (i - t - 1));
      q.tab[i - t - 1] -= 1;
    }
  }
  return make_pair(q, x);
}

/**
 * @brief Opérateur de shift a gauche
 * @param n nombre de bits a shifter
 */
Bignum &Bignum::operator<<=(unsigned n) {
  if (n == 0)
    return *this;
  unsigned n_bits = n % 32;
  unsigned n_blocks = n / 32;
  if (n_bits == 0) {
    tab.resize(tab.size() + n_blocks);
    for (unsigned i = tab.size(); i-- != n_blocks;)
      tab[i] = tab[i - n_blocks];
  } else {
    tab.resize(tab.size() + n_blocks + 1);
    for (unsigned i = tab.size(); i-- != n_blocks + 1;)
      tab[i] = (tab[i - n_blocks] << n_bits) |
               (tab[i - (n_blocks + 1)] >> (32 - n_bits));
    tab[n_blocks] = tab[0] << n_bits;
  }
  for (unsigned i = 0; i < n_blocks; ++i)
    tab[i] = 0u;

  unsigned ntab = tab.size();
  while (ntab > 1 && tab[ntab - 1] == 0)
    --ntab;
  tab.resize(ntab);

  return *this;
}

/**
 * @brief Opérateur de shift a droite
 * @param n nombre de bits a shifter
 */
Bignum &Bignum::operator>>=(unsigned n) {
  if (n == 0)
    return *this;
  unsigned n_bits = n % 32;
  unsigned n_blocks = n / 32;
  if (n_bits == 0) {
    for (unsigned i = 0; i + n_blocks < tab.size(); ++i)
      tab[i] = tab[i + n_blocks];
    if (tab.size() > n_blocks)
      tab.resize(tab.size() - n_blocks);
    else {
      tab.resize(1);
      tab[0] = 0;
    }
  } else {

    for (unsigned i = 0; i + 1 + n_blocks < tab.size(); ++i)
      tab[i] = (tab[i + n_blocks] >> n_bits) |
               (tab[i + (n_blocks + 1)] << (32 - n_bits));
    tab[tab.size() - n_blocks - 1] = tab[tab.size() - 1] >> n_bits;
  }

  unsigned ntab = tab.size();
  while (ntab > 1 && tab[ntab - 1] == 0)
    --ntab;
  tab.resize(ntab);

  return *this;
}

/**
 * @brief Opérateur de comparaison x < y

 * @return true si x < y
 * @return false si x >= y
 */
bool operator<(Bignum const &x, Bignum const &y) {
  if (x.isPositive != y.isPositive)
    return y.isPositive;
  unsigned n = x.size() - 1, t = y.size() - 1;
  while (n > 0 && x[n] == 0)
    --n;
  while (t > 0 && y[t] == 0)
    --t;
  if (n != t)
    return x.isPositive ^ (n > t);
  while (n != 0) {
    if (x[n] != y[n])
      return x.isPositive ^ (x[n] > y[n]);
    --n;
  }
  if (x[0] == y[0])
    return false;
  return x.isPositive ^ (x[0] > y[0]);
}

bool operator<=(Bignum const &x, Bignum const &y) {
  if (x.isPositive != y.isPositive)
    return y.isPositive;
  unsigned n = x.size() - 1, t = y.size() - 1;
  while (n > 0 && x[n] == 0)
    --n;
  while (t > 0 && y[t] == 0)
    --t;
  if (n != t)
    return x.isPositive ^ (n > t);
  while (n != 0) {
    if (x[n] != y[n])
      return x.isPositive ^ (x[n] > y[n]);
    --n;
  }
  if (x[0] == y[0])
    return true;
  return x.isPositive ^ (x[0] > y[0]);
}

/**
 * @brief Opérateur d'égalité
 *
 * @return true si x == y
 * @return false x != y
 */
bool operator==(Bignum const &x, Bignum const &y) {
  unsigned n = x.size() - 1, t = y.size() - 1;
  while (n > 0 && x[n] == 0)
    --n;
  while (t > 0 && y[t] == 0)
    --t;
  if (n != t)
    return false;
  if (n != 0 && x.isPositive != y.isPositive)
    return false;
  while (n != 0) {
    if (x[n] != y[n])
      return false;
    --n;
  }
  return (x[0] == y[0]) && (x.isPositive == y.isPositive || x[0] == 0);
}

/**
 * @brief Divise x par y et retourne le quotient
 * @see division
 *
 * @param x numerateur
 * @param y denominateur
 * @return Quotient de x/y
 */
Bignum operator/(Bignum const &x, Bignum const &y) {
  auto xx = x;
  xx.isPositive = true;
  auto yy = y;
  yy.isPositive = true;
  auto p = division(std::move(xx), std::move(yy));
  if (x.isPositive != y.isPositive)
    p.first.isPositive = false;
  return p.first;
}

/**
 * @brief Divise x par y et retourne le reste
 * @see division
 *
 * @param x numerateur
 * @param y denominateur
 * @return Reste de x/y
 */
Bignum operator%(Bignum const &x, Bignum const &y) {
  auto xx = x;
  xx.isPositive = true;
  auto yy = y;
  yy.isPositive = true;
  auto p = division(std::move(xx), std::move(yy));
  if (!x.isPositive) {
    p.second.isPositive = false;
    if (y.isPositive)
      p.second += y;
    else
      p.second -= y;
  }
  return p.second;
}

/**
 * @brief Exponentiation modulaire rapide
 * @link https://en.wikipedia.org/wiki/Modular_exponentiation
 *
 * @param base base
 * @param exp exposant
 * @param mod modulo
 * @return Resultat de l'exponentiation modulaire
 * @throw std::runtime_error si base, exp ou mod est negatif
 */
Bignum modpow(Bignum base, Bignum exp, Bignum &mod) {
  if (base < 0 || exp < 0 || mod < 0) {
    cerr << "Erreur --> (Exponentiation Modulaire) paramètre négatif " << endl;
    exit(1);
  }
  if (mod == 1) {
    return Bignum(0);
  }
  if (base == 1) {
    return Bignum(1);
  }
  Bignum result(1);
  base = base % mod;
  while (exp > 0) {
    if (exp[0] % 2 == 1) {
      result = (result * base) % mod;
    }
    exp = exp >> 1;
    base = (base * base) % mod;
  }

  return result;
}

/**
 * @brief Test de primalité de Fermat pour n avec nb_test tests
 * @note Selon https://math.dartmouth.edu/~carlp/PDF/paper88.pdf, 
 * pour entier de 1024 bits, 6 tests donnent un taux d'erreur de 10^(-40) 
 * @note Moins optimisé que Miller-Rabin
 *
 * @param n le bignum a tester
 * @param nb_test le nombre de test a effectuer

 * @return true, n est fort probablement premier
 * @return false, n est composé
 */
bool primality_fermat(Bignum const &n, uint nb_test) {
  if (n < 3)
    return false;
  if (n % 2 == 0)
    return false;

  Bignum tmp = n;

  for (uint i = 0; i < nb_test; i++) {
    Bignum a = rand() % (n - 2) + 2;
    if (modpow(a, tmp - 1, tmp) != 1)
      return false;
  }
  return true;
}

/**
 * @brief Calcule l'inverse de a modulo b
 * @note Utilise l'algorithme d'Euclide étendu
 * @link https://cp-algorithms.com/algebra/extended-euclid-algorithm.html
 *
 * @return Inverse de a modulo b
 * @throw std::runtime_error si b == 0
 */
Bignum invMod(Bignum const &a, Bignum const &b) {
  Bignum x1 = 1;
  Bignum x2 = 0;
  Bignum y1 = a;
  Bignum y2 = b;

  if (b == Bignum(0)) {
    cerr << "Erreur --> (Inversion modulaire) b==0" << endl;
    exit(1);
  }

  while (y2 != 0) {
    Bignum q = y1 / y2;
    Bignum rs = y1;
    Bignum us = x1;
    y1 = y2;
    x1 = x2;

    y2 = rs - (q * y1);
    x2 = us - (q * x1);
  }

  if (x1 < Bignum(0)) {
    x1 = x1 + b;
  }

  return x1;
}

/**
 * @brief Encrypt un bignum pour rsa
 *
 * @param x Message converti en bignum
 * @param e Exposant de chiffrement de la clef publique
 * @param n Modulo de chiffrement de la clef publique

 * @return Bignum chiffré
 */
Bignum encrypt(Bignum &x, Bignum &e, Bignum &n) { return modpow(x, e, n); }

/**
 * @brief Encrypt un bignum pour rsa
 *
 * @param x Message converti en bignum chiffré
 * @param d Exposant de chiffrement de la clef privée
 * @param n Modulo de chiffrement de la clef privée

 * @return Bignum déchiffré
 */
Bignum decrypt(Bignum &cypher, Bignum &d, Bignum &n) {
  return modpow(cypher, d, n);
}

/**
  * @brief Affiche un bignum sous forme décimale
 */
ostream &operator<<(ostream &flux, Bignum const &x) {
  if (x == Bignum(0))
    flux << 0;
  else {
    if (!x.isPositive)
      flux << "-";
    auto p = division(x, 10);
    p.first.isPositive = true;
    if (p.first != Bignum(0))
      flux << p.first;
    flux << p.second[0];
  }
  return flux;
}
