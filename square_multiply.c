#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>

#include "square_multiply.h"

#define color(param) printf("\033[%sm", param)
#define PUBLIC_KEY_SIZE (unsigned int) mpz_sizeinbase(e, 2)

void square_and_multiply(const mpz_t x, const mpz_t e, const mpz_t n)
{
    unsigned int i;
    mpz_t m, y, rshiftr, andr;
    mpz_init(m);
    mpz_init(y);
    mpz_init(rshiftr);
    mpz_init(andr);
    mpz_set_ui(m,1);
    mpz_set_ui(y,1);

    gmp_printf("\tparam : x = %Zd, e = ", x);
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
    gmp_printf("\tres : y = %Zd mod %Zd\n\n", y, n);

    mpz_clear(m);
    mpz_clear(y);
    mpz_clear(rshiftr);
    mpz_clear(andr);
}


void dechif(const mpz_t x, unsigned int d, const mpz_t n){

    mpz_t y;
    mpz_init(y);
    mpz_set_ui(y,0);
   

    mpz_powm_ui( y, x, d, n);

    gmp_printf("\tres : x = %Zd mod %Zd\n\n", y, n);

   mpz_clear(y);
   


}

int pgcd(int a, int b)
{
    if (b != 0)
       return pgcd(b, a%b);
    else 
       return a;
}


unsigned int generer_cle_publique_N(unsigned int p, unsigned int q)

{
     printf("Entrez p q : \n");
     scanf("%d %d",&p,&q );

while(true){
   
    if (pgcd(p,q) == 1)
    {
        return p*q;
    }

    else
    {
         printf("\n\tp %d et q %d ne sont pas premiers entre eux \n", p, q);
            printf("Entrez p q : \n");
            scanf("%d %d",&p,&q );

    }
  }
}



unsigned int generer_cle_publique_e(unsigned int p, unsigned int q){

    printf("Entrez p q pour generer exposant publique e \n");
    scanf("%d %d",&p,&q );

    unsigned int e;
    
    printf("Entrez e : \n");
     scanf("%d",&e );

while(true){
   
    if (pgcd(e,(p-1)*(q-1)) == 1)
    {
        return e;
    }

    else
    {
         printf("\n\te %d et Phi(N) %d ne sont pas premiers entre eux \n", e, (p-1)*(q-1));
            printf("Entrez e : \n");
            scanf("%d",&e);

    }
  }
}


/*unsigned int deduire_cle_secrete_d(unsigned int p,unsigned int q, unsigned int e){

    
}
*/

