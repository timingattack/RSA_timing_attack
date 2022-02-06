#ifndef __DECHIFFREMENT__
#define __DECHIFFREMENT__
#include <gmp.h>

void dechiffrement_RSA(const mpz_t c, const mpz_t  d, const mpz_t n, mpz_t m);

#endif /*__DECHIFFREMENT__*/