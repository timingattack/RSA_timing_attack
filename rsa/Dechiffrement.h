#ifndef __DECHIFFREMENT__
#define __DECHIFFREMENT__
#include <gmp.h>

void dechiffrement_RSA( mpz_t decrypt,  mpz_t x,  unsigned int d,  mpz_t n);
void Montgomery_Exponentiation_decrypt(mpz_t decrypt, mpz_t a, mpz_t u, mpz_t v,unsigned int d , mpz_t r, mpz_t n);

#endif /*__DECHIFFREMENT__*/