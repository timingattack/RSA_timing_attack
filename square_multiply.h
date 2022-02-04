#ifndef __SQUARE_MULTIPLY__
#define __SQUARE_MULTIPLY__
#include <gmp.h>

void square_and_multiply(const mpz_t x, const mpz_t e, const mpz_t n);
void dechif(const mpz_t x, unsigned int  d, const mpz_t n);
unsigned int generer_cle_publique_N(const unsigned int p, const unsigned int q);
//unsigned int deduire_cle_secrete_d(unsigned int p, unsigned int q, unsigned int e);
unsigned int generer_cle_publique_e(const unsigned int p, const unsigned int q);

int pgcd(int a, int b);



#endif /*__SQUARE_MULTIPLY__*/