#ifndef RSA
#define RSA
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void chiffrement(mpz_t clair, mp_bitcnt_t bits);
void gen_random(mpz_t tmp, gmp_randstate_t state, mp_bitcnt_t bits);
void calcul_phi(mpz_t phi, mpz_t p, mpz_t q);
void gen_key(mp_bitcnt_t bits);
void square_and_multiply(mpz_t m, mpz_t e, mpz_t n);
void dechiffrement(mpz_t clair, mp_bitcnt_t bits);
void signature(mpz_t clair, mp_bitcnt_t bits);

#endif