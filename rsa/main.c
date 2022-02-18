#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "square_multiply.h"
#include "Dechiffrement.h"
#include "Creation_des_cles.h"
#include "montgomery.h"



int main(int argc, char const *argv[])
{    

  unsigned int d;

  mpz_t u,v,r,n,pgcd,p,q,phi,zero,message,chiffre_mon,chiffre_,dechiffre_mon,dechiffre_,e;

  mpz_inits(u,v,r,n,pgcd,p,q,phi,zero,message,chiffre_mon,chiffre_,dechiffre_mon,dechiffre_,e,NULL);

  int i = 0;

    printf("\n\t Entrez vos clés secretes p q pour generer les cles publiques : \n");

    gmp_scanf("%Zd %Zd",p,q );

    mpz_set_ui(n, generer_cle_publique_N(p,q));

    gmp_printf("\n Generer la clé publique N ( N = p*q ) : %Zd\n\n",n);

    mpz_set_ui(e, generer_cle_publique_e(p,q));

    gmp_printf("\n Generer l'exposant public e : %Zd\n\n",e);

    printf("\n\t Entrez votre message : \n");

    gmp_scanf("%Zd",message);

   
    while (mpz_cmp(n, message) < 0) 
    {
        printf("\n\t Entrez un autre message ( la taille du message est superieure a n : \n");

        gmp_scanf("%Zd",message);
    }

    gmp_printf("\t- CHIFFREMENT CLASSIQUE -\n");

    gmp_printf("\tparam : clair = %Zd, e = %Zd\n",message,e);

    square_and_multiply(chiffre_,message, e, n);

    gmp_printf("\t- DECHIFFREMENT CLASSIQUE -\n");

    d = generer_cle_privee_d(e,phi,p,q);

    gmp_printf("\tparam : chiffre  = %Zd, d = %d\n", chiffre_,d );

    dechiffrement_RSA(dechiffre_,chiffre_,d,n);

 printf("------------ MONTGOMERY -------------\n");

    while (mpz_cmp(r, n) < 0) {

    mpz_ui_pow_ui(r,2,i++);

    }
     

 printf("------------ THEOREME DE BEZOUT -------------\n");

 bezout(r,n,u,v,pgcd,zero);

 gmp_printf("R(%Zd) * v(%Zd) + N(%Zd) * u(%Zd)  \n\n",r,u,n,v);

 gmp_printf("\t- CHIFFREMENT MONTGOMERY -\n");

 gmp_printf("\tparam : clair = %Zd, e = %Zd\n\n",message,e);

 Montgomery_Exponentiation_crypt(chiffre_mon,message,u,v,e,r,n);

 gmp_printf(" Le message chiffre obtenu est : %Zd mod %Zd\n\n", chiffre_mon,n);

 gmp_printf("\t- DECHIFFREMENT MONTGOMERY -\n");

 d = generer_cle_privee_d(e,phi,p,q);

 gmp_printf("\tparam : chiffre  = %Zd, d = %d\n\n", chiffre_mon,d );

 Montgomery_Exponentiation_decrypt(dechiffre_mon,chiffre_mon,u,v,d,r,n);

 gmp_printf(" Le message clair obtenu est : %Zd mod %Zd\n\n", dechiffre_mon,n);


  mpz_clears(u,v,r,n,pgcd,p,q,phi,zero,message,chiffre_mon,chiffre_,dechiffre_mon,dechiffre_,e,NULL);

    exit(0);
}

