#ifndef __MONTGOMERY__
#define __MONTGOMERY__
#include <gmp.h>

void bezout(mpz_t r, mpz_t n, mpz_t u, mpz_t v, mpz_t pgcd, mpz_t x);
unsigned int montgomery_essai(mpz_t u, mpz_t v, mpz_t z, mpz_t r, mpz_t n,mpz_t pgcd);
unsigned int verification(mpz_t u, mpz_t z, mpz_t n);
unsigned int Montgomery_product(mpz_t u, mpz_t v,mpz_t a_bar, mpz_t b_bar, mpz_t r, mpz_t n);
unsigned int Multiplication_modulaire_montgomery(mpz_t u, mpz_t v,mpz_t a, mpz_t b, mpz_t r, mpz_t n);
void Montgomery_Exponentiation_crypt(mpz_t crypt, mpz_t a, mpz_t u, mpz_t v, mpz_t e , mpz_t r, mpz_t n);

#endif /*__MONTGOMERY__*/