#ifndef __MONTGOMERY__
#define __MONTGOMERY__
#include <gmp.h>

void bezout(mpz_t a, mpz_t b, mpz_t u, mpz_t v, mpz_t pgcd, mpz_t lastV);
void montgomery_essai(mpz_t u, mpz_t v, mpz_t z, mpz_t r, mpz_t n,mpz_t pgcd);
#endif /*__MONTGOMERY__*/