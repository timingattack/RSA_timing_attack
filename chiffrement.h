#ifndef __CHIFFREMENT__
#define __CHIFFREMENT__
#include <gmp.h>

void chiffrement_RSA(const mpz_t c, const mpz_t  d, const mpz_t n, mpz_t m);

#endif /*__CHIFFREMENT__*/