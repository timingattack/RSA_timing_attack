#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include "Dechiffrement.h"
#include "square_multiply.h"


unsigned int dechiffrement_RSA(const mpz_t x, unsigned int d, const mpz_t n){

    mpz_t y;
    mpz_init(y);
    mpz_set_ui(y,0);
   
        gmp_printf("--------------------\n");

        gmp_printf("\t- DECHIFFREMENT -\n");



    mpz_powm_ui( y, x, d, n);

    gmp_printf("\t x = %Zd mod %Zd\n\n", y, n);

 
   return mpz_get_ui(y);

     mpz_clear(y);
   

}
