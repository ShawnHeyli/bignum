#include "../lib/Bignum.hpp"
#include "../lib/rsa.hpp"
#include "../lib/progressbar.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

struct RSAResult {
  string message; /// Message original
  vector<Bignum> cypher;
  string plaintext; /// Message déchiffré
  pair<chrono::duration<double>, chrono::duration<double>>
      time; /// Temps de génération des clés et de chiffrement/déchiffrement
};

/**
 * @brief fonction d'aide pour tester RSA
 *
 * @param nbit la taille du nombre en bit
 * @param message le message a chiffrer
 * @return RSAResult
 */
RSAResult rsa(int nbit, string message) {
  chrono::time_point<chrono::system_clock> start, end;
  chrono::duration<double> time_p_q;
  chrono::duration<double> time_cypher_decypher;

  start = chrono::system_clock::now();
  Bignum p = prime(nbit);
  Bignum q = prime(nbit);
  end = chrono::system_clock::now();
  time_p_q = end - start;

  auto keys = keygen(p, q);

  start = chrono::system_clock::now();
  vector<Bignum> cypher = encode_vector(message, keys.first.e, keys.first.n);
  string plain_decrypted = decode_vector(cypher, keys.second.d, keys.second.n);
  end = chrono::system_clock::now();
  time_cypher_decypher = end - start;

  return RSAResult{message, cypher, plain_decrypted,
                   make_pair(time_p_q, time_cypher_decypher)};
}

int main() {
  srand((unsigned)time(NULL));
  int choice = 0;
  while (choice != 3) {
    cout << "1. Chiffrement RSA" << endl;
    cout << "2. Benchmark" << endl;
    cout << "3. Quitter" << endl;
    cout << "Choix: ";
    cin >> choice;
    cout << endl;

    chrono::time_point<chrono::system_clock> start, end;
    chrono::duration<double> average_p_q = chrono::duration<double>::zero();
    chrono::duration<double> average_cypher_decypher = chrono::duration<double>::zero();
    int nb_test = 100;
    progressbar bar(nb_test);

    int nbit;
    
    RSAResult rsa_result;
    string message =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras nec "
        "dolor at erat pharetra egestas ac a nisi. Vestibulum vestibulum felis "
        "tellus, in malesuada orci vulputate sed. Vestibulum rutrum sagittis "
        "elit eu placerat. Pellentesque gravida dui non mollis imperdiet. "
        "Vestibulum magna neque, laoreet vitae placerat in, rutrum sit amet "
        "enim. Praesent convallis egestas neque in eleifend. Donec et felis "
        "mi. Duis rutrum lorem at est suscipit vestibulum. Nulla facilisi. ";

    switch (choice) {
    case 1:
      cout << "nbit (ex: 256,512,1024): ";
      cin >> nbit;
      cout << endl;
      rsa_result = rsa(nbit, "Hi mom!");
      cout << "Message: " << rsa_result.message << endl;
      cout << "Cypher: ";
      for (auto c : rsa_result.cypher) {
        cout << c << " ";
      }
      cout << endl;
      cout << "Plaintext: " << rsa_result.plaintext << endl;
      cout << "Temps de génération des clés: " << rsa_result.time.first.count()
           << "s" << endl;
      cout << "Temps de chiffrement/déchiffrement: "
           << rsa_result.time.second.count() << "s" << endl;
      cout << endl;
      break;

    case 2:
      cout << "nbit (ex: 256,512,1024): ";
      cin >> nbit;
      cout << endl;

      
      start = chrono::system_clock::now();
      for (int i = 0; i < nb_test; i++) {
        rsa_result = rsa(nbit, message);
        bar.update();

        average_p_q += rsa_result.time.first;
        average_cypher_decypher += rsa_result.time.second;
      }
      end = chrono::system_clock::now();
      cout << endl;
      cout << "Temps moyen de génération des clés: "
           << average_p_q.count() / nb_test << "s" << endl;
      cout << "Temps moyen de chiffrement/déchiffrement: "
            << average_cypher_decypher.count() / nb_test << "s" << endl;
      break;

    case 3:
      exit(EXIT_SUCCESS);
      break;

    default:
      cerr << "Choix invalide" << endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}