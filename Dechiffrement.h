#ifndef __DECHIFFREMENT__
#define __DECHIFFREMENT__
#include <gmp.h>

unsigned  int dechiffrement_RSA(const mpz_t x, unsigned int  d, const mpz_t n);

#endif /*__DECHIFFREMENT__*/