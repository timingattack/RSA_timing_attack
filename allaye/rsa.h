#ifndef RSA
#define RSA
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void chiffrement(mpz_t clair, mp_bitcnt_t bits, int choix);
void gen_random(mpz_t tmp, gmp_randstate_t state, mp_bitcnt_t bits);
void calcul_phi(mpz_t phi, mpz_t p, mpz_t q);
void gen_key(mp_bitcnt_t bits);
void square_and_multiply(mpz_t m, mpz_t e, mpz_t n);
void dechiffrement(mpz_t clair, mp_bitcnt_t bits, int choix);
void signature(mpz_t clair, mp_bitcnt_t bits, int choix);
void calcul_R(mpz_t r, mpz_t n);
void mult_montgomery(mpz_t result, mpz_t a, mpz_t b, mpz_t n, mpz_t r, mpz_t inv_r);
void exponentiation_modulaire(mpz_t m, mpz_t e, mpz_t n);
void reduction_montgomery(mpz_t result, mpz_t a, mpz_t b, mpz_t r, mpz_t inv_r, mpz_t inv_n);
void calcul_bezout(mpz_t inv_r, mpz_t inv_n, mpz_t r, mpz_t n);
void reduc_montgomery(mpz_t result, mpz_t a, mpz_t b, mpz_t n, mpz_t inv_n, mpz_t r, mpz_t inv_r);
#endif
