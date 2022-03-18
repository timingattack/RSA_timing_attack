#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include "Dechiffrement.h"
#include "square_multiply.h"
#include "montgomery.h"


void  dechiffrement_RSA(mpz_t decrypt, mpz_t x,  unsigned int d,  mpz_t n){

	mpz_t inv;
	mpz_init(inv);
	mpz_set_ui(inv,d);

  	square_and_multiply(decrypt ,x,  inv,  n); 

    gmp_printf("--------------------\n");

 	mpz_clear(inv);

}

void Montgomery_Exponentiation_decrypt(mpz_t decrypt, mpz_t a, mpz_t u, mpz_t v, unsigned int d, mpz_t r, mpz_t n){


	mpz_t inv;
	mpz_init(inv);
	mpz_set_ui(inv,d);

  	Montgomery_Exponentiation_crypt(decrypt ,a,u, v, inv ,  r, n); 

    gmp_printf("--------------------\n");

 	mpz_clear(inv);

}


