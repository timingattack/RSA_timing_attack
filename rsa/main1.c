#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "square_multiply.h"
#include "Dechiffrement.h"
#include "Creation_des_cles.h"
#include "montgomery.h"



int main(int argc, char const *argv[])
{    

  
/*  unsigned int t,verif, essai,d;

  mpz_t u,v,r,z,n,p,zero,b,a_bar,b_bar,e;

  mpz_inits(u,v,r,z,n,p,zero,b,a_bar,b_bar,e,NULL);

  mpz_init_set_ui(z,1);
  mpz_ui_pow_ui(r,2,4);
  
  mpz_init_set_ui(n,15);
  mpz_init_set_ui(a_bar,48);
  mpz_init_set_ui(b_bar,80);
  mpz_init_set_ui(b,7);
  mpz_init_set_ui(e,10);*/




//mpz_set_ui(r,essai);

 /* mpz_init(v);
  mpz_init(p);
  mpz_init_set_ui(zero, 0);*/
  
  /* --- RU + NV = 1 */

/*bezout(r,n,u,v,p,zero);

gmp_printf("Ru = %Zd ( %Zd )+ Nv = %Zd ( %Zd )  \n\n",r,u,n,v);*/

 /* --- RU + NV = 1 */

// t = montgomery_essai(u,v,z,r,n,p);

  
/*essai = ModExp(b,u,v,e,r,n);

printf("t = %d\n",essai);*/



  
        
    exit(0);
}










  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@












#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "square_multiply.h"
#include "Dechiffrement.h"
#include "Creation_des_cles.h"
#include "montgomery.h"



int main(int argc, char const *argv[])
{    

  
    unsigned int   d;
     
    mpz_t p,q,n,e,phi,message,gen,y;

    mpz_inits(p,q,n,e,phi,message,gen,y, NULL);

    printf("\n\t Entrez vos clés secretes p q pour generer exposant public e : \n");

    gmp_scanf("%Zd %Zd",p,q );

    printf("\n Generer la clé publique N ( N = p*q ) : \n");

    mpz_set_ui(n, generer_cle_publique_N(p,q));

    printf("\n Generer l'exposant public e : \n");

    mpz_set_ui(e, generer_cle_publique_e(p,q));
   
    printf("\n\t Entrez votre message : \n");

    gmp_scanf("%Zd",message);

   
    while (mpz_cmp(n, message) < 0) 
    {
        printf("\n\t Entrez un autre message ( la taille du message est superieure a n : \n");

        gmp_scanf("%Zd",message);
    }

    gmp_printf("\t- CHIFFREMENT -\n");

    gmp_printf("\tparam : clair = %Zd, e = %Zd\n",message,e);

    square_and_multiply(gen,message, e, n);

    gmp_printf("\t- DECHIFFREMENT -\n");

    d = generer_cle_privee_d(e,phi,p,q);

    gmp_printf("\tparam : chiffre  = %Zd, d = %d\n", gen,d );

    dechiffrement_RSA(y,gen,d,n);



        
    exit(0);
}