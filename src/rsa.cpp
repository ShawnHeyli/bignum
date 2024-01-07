#include "../lib/rsa.hpp"
#include "../lib/Bignum.hpp"
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

/**
 * @brief Génère un nombre aléatoire de nbit
 * @param nbit si nbit < 32 alors nbit = 32 (nbyte = 1)
 * @return Bignum
 */
Bignum random_bignum(int const nbit) {
  int nbyte = nbit / 32;
  if(nbyte < 1) nbyte = 1;
  Bignum n(0);
  for (int i = 0; i < nbyte; i++) {
    n.tab.emplace_back(rand());
  }
  // Rend le bignum impaire (on évite les nombres pair qui ne sont pas premier)
  n.tab[0] = n.tab[0] % 2 ? n.tab[0] : n.tab[0] + 1;
  return n;
}

/**
  * @brief Génère un nombre premier de nbit
  * @note 1 / (ln(2^nbit)) = probabilité que le nombre soit premier (ex: 1 / (ln(2^1024)) = (1 / 710))
  * @return Un bignum fortement probablement premier
 */
Bignum prime(const int nbit)
{
  Bignum n = random_bignum(nbit);
  while (!primality_fermat(n, 20))
    n = n + 2; // On passe au nombre impair suivant
  return n;
}

/**
 * Génère une paire de clé RSA.
 * 
 * @param p Un nombre premier.
 * @param q Un nombre premier.
 * @return Une paire de clé RSA.
 * @throws std::runtime_error si p == q ou si p ou q n'est pas premier.
 */
pair<PublicKey, PrivateKey> keygen(Bignum const &p, Bignum const &q) {
  if (p == q) {
    cerr << "Erreur --> (keygen) p et q doivent être différent" << endl;
    exit(EXIT_FAILURE);
  }
  if (!primality_fermat(p, 20) || !primality_fermat(q, 20)) {
    cerr << "Erreur --> (keygen) p et q doivent être premier" << endl;
    exit(EXIT_FAILURE);
  }
  Bignum n = p * q;
  Bignum phi = (p - 1) * (q - 1);
  Bignum e = 65537;
  Bignum d = invMod(e, phi);
  return make_pair(PublicKey{e, n}, PrivateKey{d, n});
}

/**
 * @brief Transforme 4 char en un bignum (4 char = 32 bit)
 * @return Un bignum représentant les 4 char
 */
Bignum encode(string input)
{
  Bignum result(0);
  for (unsigned long i = 0; i < input.length(); i++)
  {
    char char_block = input[i];
    result <<= 8;
    result += char_block;
  }
  return result;
}

/**
 * @brief Transforme un bignum en 4 char (32 bit = 4 char)
 * @return Un string représentant le bignum
 */
string decode(Bignum input)
{
  string result = "";
  while (input > 0)
  {
    char char_block = input[0] % 256;
    result.insert(result.begin(), char_block);
    input >>= 8;
  }
  return result;
}

/**
 * @brief chiffre un message un vecteur de chiffrement (si la longeur du
 * message est supperieur a n, on a besoin de plusieur chiffre)
 *
 * @param message le message a chiffrer
 * @param e exposant de chiffrement
 * @param n module de chiffrememnt
 * @return vector<Bignum>
 */
vector<Bignum> encode_vector(const string message, Bignum &e, Bignum &n)
{
  unsigned long block_size = (n.size() * 4) - 1;
  unsigned long message_length = message.length();
  vector<Bignum> cypher;
  for (unsigned long i = 0; i < message_length; i += block_size)
  {
    string block = message.substr(i, block_size);
    Bignum encode_block = encode(block);
    Bignum encrypted_block = encrypt(encode_block, e, n);
    cypher.emplace_back(encrypted_block);
  }
  return cypher;
}

/**
 * @brief dechiffre une liste de chiffre
 *
 * @param cypher la liste de chiffre
 * @param d l'exposant de dechiffrement
 * @param n le module de chiffrement
 * @return string
 */
string decode_vector(vector<Bignum> cypher, Bignum &d, Bignum &n)
{
  string plaintext = "";
  for (unsigned long i = 0; i < cypher.size(); i++)
  {
    Bignum c(cypher[i]);
    Bignum decrypted_block = decrypt(c, d, n);
    plaintext += decode(decrypted_block);
  }
  return plaintext;
}