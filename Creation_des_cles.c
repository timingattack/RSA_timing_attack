#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include "Creation_des_cles.h"
#include "square_multiply.h"





int pgcd(int a, int b)
{
    if (b != 0)
       return pgcd(b, a%b);
    else 
       return a;
}


unsigned int generer_cle_publique_N(unsigned int p, unsigned int q)

{
    printf("\n Generer la clé publique N ( N = p*q ) : \n");

    printf("\n\t Veuillez Entrez vos clés secretes p q : \n");

    scanf("%d %d",&p,&q );

    mpz_t p1;
    mpz_init(p1);
    mpz_set_ui(p1,p);
    mpz_t q1;
    mpz_init(q1);
    mpz_set_ui(q1,q);

while(true){
   
    if (mpz_probab_prime_p(p1,p) != 2 && mpz_probab_prime_p(q1,q) != 2)
    {
       printf("\n\tp %d et q %d ne sont pas premiers \n", p, q);
            printf("Entrez p q : \n");
            scanf("%d %d",&p,&q );
    }

    if (mpz_probab_prime_p(p1,p) != 2)
    {
        printf("\n\tp %d n'est pas premier \n", p);
            printf("Entrez un autre q : \n");
            scanf("%d",&p );
    }

    if (mpz_probab_prime_p(q1,q) != 2)
    {
        printf("\n\t q %d n'est pas premier \n", q);

            printf("Entrez un autre q : \n");

            scanf("%d",&q );
    }

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

     mpz_clear(q1);
     mpz_clear(p1); 

}



unsigned int generer_cle_publique_e(unsigned int p, unsigned int q){

    
    printf("\n Generer l'exposant public e : \n");

    printf("\n\t Entrez vos clés secretes p q pour generer exposant public e : \n");

    scanf("%d %d",&p,&q );

    unsigned int e;
    
    printf("\n Choisissez un entier naturel e : \n");

    scanf("%d",&e );

while(true){
   
    if ((pgcd(e,(p-1)*(q-1)) == 1) && (e < (p-1)*(q-1)) ) 
    {
        return e;
    }

    if (e >= (p-1)*(q-1))
    {
        printf("\n\te %d est superieur a Phi(N) %d \n", e, (p-1)*(q-1));

            printf("\nEntrez un nouveau e : \n");
            scanf("%d",&e);
    }
    else
    {
         printf("\n\te %d et Phi(N) %d ne sont pas premiers entre eux \n", e, (p-1)*(q-1));

            printf("\nEntrez un nouveau e : \n");
            scanf("%d",&e);

    }
  }
}



