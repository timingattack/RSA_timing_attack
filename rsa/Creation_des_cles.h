#ifndef __CREATION_DES_CLES__
#define __CREATION_DES_CLES__
#include <gmp.h>


unsigned int generer_cle_publique_N(mpz_t p, mpz_t q);
unsigned int generer_cle_publique_e(mpz_t p, mpz_t q);
unsigned int pgcd( unsigned int a,  unsigned int b);
unsigned int generer_cle_privee_d(mpz_t e, mpz_t phi, mpz_t p, mpz_t q);



#endif /*__CREATION_DES_CLES__*/