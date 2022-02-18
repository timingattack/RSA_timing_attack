#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include "Creation_des_cles.h"
#include "square_multiply.h"

unsigned int pgcd(unsigned int a, unsigned int b)
{
    if (b != 0)
       return pgcd(b, a%b);
    else 
       return a;
}


unsigned int generer_cle_publique_N(mpz_t p, mpz_t q)

{
    
     mpz_t n;
    mpz_init(n);
    mpz_mul(n,p,q);

while(true){
   
    if (mpz_probab_prime_p(p,mpz_get_ui(p)) != 2 && mpz_probab_prime_p(q,mpz_get_ui(q)) != 2)
    {
       gmp_printf("\n\tp = %Zd et q = %Zd ne sont pas premiers \n", p, q);
            printf("Entrez p q : \n");
            gmp_scanf("%Zd %Zd",p,q );
    }

    if (mpz_probab_prime_p(p,mpz_get_ui(p)) != 2)
    {
        gmp_printf("\n\tp = %Zd n'est pas premier \n", p);
            printf("Entrez un autre q : \n");
            gmp_scanf("%Zd",p );
    }

    if (mpz_probab_prime_p(q,mpz_get_ui(q)) != 2)
    {
        gmp_printf("\n\t q = %Zd n'est pas premier \n", q);

            printf("Entrez un autre q : \n");

            gmp_scanf("%Zd",q );
    }

   
   

    if (pgcd(mpz_get_ui(p),mpz_get_ui(q)) == 1)
    {
        return mpz_get_ui(n);
    }

    else
    {
         gmp_printf("\n\tp = %Zd et q = %Zd ne sont pas premiers entre eux \n", p, q);
            printf("Entrez p q : \n");
            gmp_scanf("%Zd %Zd",p,q );

    }
  }

  mpz_clear(n);
}



unsigned int generer_cle_publique_e(mpz_t p, mpz_t q){

    mpz_t e;
    mpz_init(e);

    printf("\n Choisissez un entier naturel e : \n");

    gmp_scanf("%Zd",e );


while(true){
    
    if (mpz_cmp_ui(e, 1) == 0)
    {
        gmp_printf("\n\te =  %Zd doit etre superieur a 1 \n", e);

            printf("\nEntrez un nouveau e : \n");
            gmp_scanf("%Zd",e);
    }

    if ((pgcd(mpz_get_ui(e),((mpz_get_ui(p))-1)*((mpz_get_ui(q))-1)) == 1) && (mpz_get_ui(e) < ((mpz_get_ui(p))-1)*((mpz_get_ui(q))-1)) ) 
    {
        return mpz_get_ui(e);
    }

    if (mpz_get_ui(e) >= ((mpz_get_ui(p))-1)*((mpz_get_ui(q))-1))
    {
        gmp_printf("\n\te = %Zd est superieur a Phi(N) %lu \n", e, ((mpz_get_ui(p))-1)*((mpz_get_ui(q))-1));

            printf("\nEntrez un nouveau e : \n");
            gmp_scanf("%Zd",e);
    }
    else
    {
         gmp_printf("\n\t e = %Zd et Phi(N) %lu ne sont pas premiers entre eux \n", e, ((mpz_get_ui(p))-1)*((mpz_get_ui(q))-1));

            printf("\nEntrez un nouveau e : \n");
            gmp_scanf("%Zd",e);

    }
  }
}


unsigned int generer_cle_privee_d(mpz_t e, mpz_t phi, mpz_t p, mpz_t q){

    unsigned int e_, d;

    mpz_t inv;
    mpz_init(inv);
    mpz_set_ui(inv,0);

    mpz_set_ui(phi,((mpz_get_ui(p))-1)*((mpz_get_ui(q))-1));

    e_ = mpz_get_ui(e);

    mpz_invert(inv, e, phi);

    d = mpz_get_ui(inv);

while(true){
   
    if ((pgcd(e_,d == 1) )) 
    {
        return d;
    }

  }

     mpz_clear(inv); 

}



