#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "square_multiply.h"

//#define PxQ(p,q) p * q

int main(int argc, char const *argv[])
{    
    unsigned int p , q ;
    
    
    mpz_t x, e, n,y;
    mpz_init(x);
    mpz_init(y);
    mpz_init(e);
    mpz_init(n);
    
    mpz_set_ui(x, 100);
    mpz_set_ui(n, generer_cle_publique_N(p,q));
    mpz_set_ui(e, generer_cle_publique_e(p,q));
    mpz_set_ui(y, 133);

    
    square_and_multiply(x, e, n);

    dechif(y,51,n);
        
    exit(0);
}