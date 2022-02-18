#ifndef __CHIFFREMENT__
#define __CHIFFREMENT__
#include <gmp.h>

void chiffrement_RSA(mpz_t m, const mpz_t  e, const mpz_t n, mpz_t c);
void signature(mpz_t m, const mpz_t d, const mpz_t n, mpz_t s);

#endif /*__CHIFFREMENT__*/
