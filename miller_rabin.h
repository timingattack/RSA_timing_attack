#ifndef __MILLER_RABIN__
#define __MILLER_RABIN__
#include <gmp.h>
#include <stdbool.h>

void generer_un_nombre_premier(mpz_t p, int b);
bool miller_rabin(const mpz_t n);
bool temoin_de_miller(const mpz_t n, const mpz_t a);

#endif /*__MILLER_RABIN__*/