#ifndef __CREATION_DES_CLES__
#define __CREATION_DES_CLES__
#include <gmp.h>

unsigned int pgcd(const unsigned int a, const unsigned int b);
void generer_npq(mpz_t n, mpz_t p, mpz_t q);
//pour chiffrer
void generer_exposant_public(const mpz_t phi_n, mpz_t e);
//pour déchiffrement
void phi(const mpz_t p, const mpz_t q, mpz_t phi);
void generer_exposant_privee(const mpz_t e, const mpz_t phi, mpz_t d);

#endif /*__CREATION_DES_CLES__*/