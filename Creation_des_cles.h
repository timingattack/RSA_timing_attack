#ifndef __CREATION_DES_CLES__
#define __CREATION_DES_CLES__
#include <gmp.h>


unsigned int generer_cle_publique_N(const unsigned int p, const unsigned int q);
//unsigned int deduire_cle_secrete_d(unsigned int p, unsigned int q, unsigned int e);
unsigned int generer_cle_publique_e(const unsigned int p, const unsigned int q);
unsigned int pgcd(const unsigned int a, const unsigned int b);



#endif /*__CREATION_DES_CLES__*/