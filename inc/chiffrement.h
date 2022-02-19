#ifndef __CHIFFREMENT__
#define __CHIFFREMENT__
#include <gmp.h>

void chiffrement_RSA(mpz_t m, const mpz_t  e, const mpz_t n, mpz_t c);
void chiffrement_RSA_montgomery(mpz_t m, const mpz_t e, const mpz_t n, mpz_t c, const mpz_t v, const mpz_t r);
void signature(mpz_t m, const mpz_t d, const mpz_t n, mpz_t s);

void generer_R_montgomery(mpz_t r, const mpz_t n);
void bezout(const mpz_t r, const mpz_t n, mpz_t u, mpz_t v, mpz_t pgcd);
void verification(const mpz_t u, const mpz_t z, const mpz_t n, mpz_t verif);
void Montgomery_product(const mpz_t v, const mpz_t a_bar, const mpz_t b_bar, const mpz_t r, const mpz_t n, mpz_t res);
void Multiplication_modulaire_montgomery(const mpz_t v, const mpz_t a, const mpz_t b, const mpz_t r, const mpz_t n, mpz_t res);
void Montgomery_Exponentiation_crypt(mpz_t crypt, const mpz_t a, const mpz_t v, const mpz_t e, const mpz_t r, const mpz_t n);

#endif /*__CHIFFREMENT__*/
