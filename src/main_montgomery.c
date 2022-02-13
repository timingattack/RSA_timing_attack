#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "square_multiply.h"
#include "dechiffrement.h"
#include "creation_des_cles.h"
#include "montgomery.h"

int main(void)
{
  mpz_t zero;
  mpz_t u;
  mpz_t v;
  mpz_t p;

  mpz_t r, z,n;

  mpz_init(zero);
  mpz_init(u);
  mpz_init(r);

  mpz_init_set_ui(z,100);
  mpz_ui_pow_ui(r,2,8);
  mpz_init_set_ui(n,179);

  mpz_init(v);
  mpz_init(p);
  mpz_init_set_ui(zero, 0);

  //RU + NV = 1
  bezout(r,n,u,v,p,zero);
  gmp_printf("Ru = %Zd ( %Zd )+ Nv = %Zd ( %Zd )  \n\n",r,u,n,v);

  //RU + NV = 1
  montgomery_essai(u,v,z,r,n,p);

  /*unsigned int p , q ,m, c, message;

  mpz_t x, e, n,y;

  mpz_init(x);
  mpz_init(y);
  mpz_init(e);
  mpz_init(n);

  printf("\n\t Entrez vos clés secretes p q pour generer exposant public e : \n");
  scanf("%d %d",&p,&q );

  printf("\n Generer la clé publique N ( N = p*q ) : \n");

  mpz_set_ui(n, generer_cle_publique_N(p,q));

  printf("\n Generer l'exposant public e : \n");

  mpz_set_ui(e, generer_cle_publique_e(p,q));

  printf("\n\t Entrez votre message : \n");

  scanf("%d",&message);

  mpz_set_ui(x, message);

  c = square_and_multiply(x, e, n);

  mpz_set_ui(x, c);

  m = dechiffrement_RSA(x,51,n);*/

  exit(0);
}
