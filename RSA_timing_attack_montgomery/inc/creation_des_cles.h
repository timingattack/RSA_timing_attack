#ifndef __CREATION_DES_CLES__
#define __CREATION_DES_CLES__
#include <gmp.h>

//différentes tailles de bits prédéfinies pour p et q
#define PRIME_VERY_LONG_SIZE 2048   //RSA 4096 bits
#define PRIME_LONG_SIZE 1536        //RSA 3072 bits
#define PRIME_MEDIUM_SIZE 1024      //RSA 2048 bits
#define PRIME_LOW_SIZE 512          //RSA 1024 bits

int prime_size;

unsigned int pgcd(const unsigned int a, const unsigned int b);
void generer_npq(mpz_t n, mpz_t p, mpz_t q);
//-------chiffrement-------
void generer_exposant_public(mpz_t e);
//------déchiffrement------
void phi(const mpz_t p, const mpz_t q, mpz_t phi);
void generer_exposant_privee(const mpz_t e, const mpz_t phi, mpz_t d);

#endif /*__CREATION_DES_CLES__*/
