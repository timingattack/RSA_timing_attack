#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>

#include "square_multiply.h"

#define color(param) printf("\033[%sm", param)
#define PUBLIC_KEY_SIZE (unsigned int) mpz_sizeinbase(e, 2)


void square_and_multiply(mpz_t y, mpz_t x,  mpz_t e,  mpz_t n)
{
    unsigned int i;
    mpz_t m, rshiftr, andr;
    mpz_init(m);
    mpz_init(rshiftr);
    mpz_init(andr);
    mpz_set_ui(m,1);
    mpz_set_ui(y,1);

    color("1");
    color("32");
    gmp_printf("%Zd", e);
    color("0");
    gmp_printf(", n = %Zd\n\n", n);

    for (i = PUBLIC_KEY_SIZE; i > 0; i--)
    {
        //square
        mpz_mul(y, y, y); mpz_mod(y, y, n);
        color("35");
        gmp_printf("square : ");
        color("0");
        gmp_printf("y = %Zd\n", y);
        
        //bit e verification
       mpz_tdiv_q_2exp(rshiftr, e, i - 1);
       mpz_and(andr, rshiftr, m);
        if(!(mpz_cmp_ui(andr, 1)))
        {    
            gmp_printf("\nbit e%d = ", i - 1);
            color("1");
            color("32");
            gmp_printf("1\n\n");
            color("0");

            //multiply
            mpz_mul(y, y, x); mpz_mod(y, y, n);
            color("36");
            gmp_printf("multiply : ");
            color("0");
            gmp_printf("y = %Zd\n", y);
        } else {
            gmp_printf("\nbit e%d = ", i - 1);
            color("1");
            color("32");
            gmp_printf("0\n");
            color("0");
        }
        gmp_printf("--------------------\n");

       
    }
    gmp_printf("\t y = %Zd mod %Zd\n\n", y, n);
   
    mpz_clear(m);
    mpz_clear(rshiftr);
    mpz_clear(andr);

       
}

