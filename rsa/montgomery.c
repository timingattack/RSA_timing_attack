#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include "Creation_des_cles.h"
#include "square_multiply.h"
#include "montgomery.h"


void bezout(mpz_t r, mpz_t n , mpz_t u, mpz_t v, mpz_t pgcd, mpz_t x)
{

                             

                 /* Applique le theoreme de Bezout sur n et r et renvoie u et v tel que 
                                      --- RU + NV = 1  --- */
                  

  if (!mpz_cmp_ui(n, 0))
    {
    	
      mpz_set(pgcd, r);

      mpz_set_ui(u, 1);

      mpz_set(v, x);
        
    }

  else
    {
      mpz_t q;
      mpz_t reste;
      mpz_t u1;
      mpz_t v1;
      mpz_t v_q;

      mpz_inits(q,reste,u1,v1,v_q,NULL);
      
      mpz_tdiv_qr(q, reste, r, n);

      bezout(n, reste, u1, v1, pgcd, x); 

      mpz_set(u, v1);

      mpz_mul(v_q, v1, q);

      mpz_sub(v, u1, v_q);

      mpz_clear(q);
      mpz_clear(reste);
      mpz_clear(u1);
      mpz_clear(v1);
      mpz_clear(v_q);
    }

   

}

unsigned int Montgomery_product(mpz_t u, mpz_t v,mpz_t a_bar, mpz_t b_bar, mpz_t r, mpz_t n){

  mpz_t t ,m ,res ,nprim , rop1, rop2, rop3;
  
  mpz_inits(t,m,res,nprim,rop1, rop2, rop3,NULL);
 
  mpz_neg(nprim,v); // nprim = -v
  
  mpz_mul(rop1,a_bar, b_bar);

  mpz_mod(t,rop1,r);

  mpz_mul(m,t,nprim);

  mpz_mod(m,m,r);

  mpz_mul(rop2, m,n);

  mpz_add(rop3, rop1, rop2);

  mpz_div(res, rop3, r);

  if (res >= n)
  {
    mpz_sub(res,res, n);
  }

  return mpz_get_ui(res);

  mpz_clears(t,m,res,nprim,rop1, rop2, rop3,NULL);
      

}

unsigned int Multiplication_modulaire_montgomery(mpz_t u, mpz_t v,mpz_t a, mpz_t b, mpz_t r, mpz_t n){

  mpz_t x_bar, x, a_bar, b_bar, rop1, rop2 , un;

  mpz_inits(x,x_bar,a_bar,b_bar, rop1,rop2,un,NULL);

  mpz_set_ui(un, 1);

  mpz_mul(rop1, a,r);

  mpz_mod(a_bar, rop1, n);

  mpz_mul(rop2,b,r);

  mpz_mod(b_bar, rop2, n);

  mpz_set_ui(x_bar, Montgomery_product(u,v,a_bar,b_bar,r,n));

  mpz_set_ui(x, Montgomery_product(u,v,x_bar,un,r,n));

  return mpz_get_ui(x);

  mpz_clears(x,x_bar,a_bar,b_bar, rop1,rop2,un,NULL);

  
}


void Montgomery_Exponentiation_crypt(mpz_t crypt ,mpz_t a, mpz_t u, mpz_t v, mpz_t e , mpz_t r, mpz_t n){

  mpz_t a_bar, x_bar, rop1, rop2,un, rshiftr, andr ,m;

  unsigned int k,taille;

  taille = mpz_sizeinbase(e, 2);

  mpz_inits(a_bar,x_bar,rop1,rop2,un, rshiftr, andr, m,NULL);

  mpz_set_ui(un,1);

  mpz_set_ui(m,1);

  mpz_mul(rop1,a,r);

  mpz_mod(a_bar,rop1, n);

  mpz_mul(x_bar,un,r);

  for (k = taille; k > 0; k--)
  {
      mpz_set_ui(x_bar, Montgomery_product(u,v,x_bar,x_bar,r,n));

       mpz_tdiv_q_2exp(rshiftr, e, k - 1);
       mpz_and(andr, rshiftr, m);
        if(!(mpz_cmp_ui(andr, 1)))
        {    
        mpz_set_ui(x_bar, Montgomery_product(u,v,a_bar,x_bar,r,n));
      }
  }

  mpz_set_ui(crypt, Montgomery_product(u,v,x_bar,un,r,n));

  mpz_clears(a_bar,x_bar,rop1,rop2,un,rshiftr, andr,m,NULL);

}


unsigned int  montgomery_essai(mpz_t u, mpz_t v, mpz_t z, mpz_t r, mpz_t n,mpz_t pgcd){


 /* --- RR^-1 + NNPRIM = 1 

      R^-1 = U 

      NPRIM = -V
 */

	mpz_t m, t, n_impair;

	mpz_t nprim; 

  mpz_inits(nprim,n_impair,m,t,NULL);

	mpz_neg(nprim,v); // nprim = -v
	
  gmp_printf("nprim = %Zd \n",nprim);


    //MONTGOMERY 

    /*

    m = ( z mod r ) * nprim mod r

    t = ( z + m * n ) / r

    si t > n --> t = t-n 

    sino  t


  	*/

  if (mpz_get_ui(pgcd) != 1)
  {
    gmp_printf("r et n ne sont pas premier entre eux pgcd = %Zd\n", pgcd);
  }

  if (mpz_cmp(r,n) <= 0)
  {
    gmp_printf("r est inferieur ou egal a n\n  r = %Zd et n = %Zd\n", r,n);
  }

  if (mpz_mod_ui(n_impair, n , 2) == 0)
  {
    gmp_printf(" n est pair n = %Zd \n",n);
    
  }

  if (mpz_cmp_ui(n,3) < 0)
  {
     gmp_printf(" n est inferieur a 3 n = %Zd \n",n);
    
  }

  if (mpz_cmp(r,n) > 0 && mpz_get_ui(pgcd) == 1 && mpz_mod_ui(n_impair, n , 2) != 0 && mpz_cmp_ui(n,3) >= 0)
  {
     mpz_mod(m,z,r); // m = ( z mod r )

    gmp_printf("( z mod r ) = %Zd \n",m);

    mpz_mul(m, m,nprim ); //  m = ( z mod r ) * nprim 

    gmp_printf("( z mod r ) * nprim  = %Zd \n",m);

    mpz_mod(m,m,r); // m = ( z mod r ) * nprim mod r

    gmp_printf("m = %Zd \n",m);

    mpz_mul(t, m,n ); // t = ( m * n )

    gmp_printf("m * n = %Zd \n",t);

    mpz_add(t,z,t); // t = ( z + m * n )

    gmp_printf("z + m * n = %Zd \n",t);

    mpz_div(t,t,r); // t = ( z + m * n ) / r

    if (t > n)
    {
      mpz_sub(t,t,n); // t > n --> t = t-n 
    }
  
      mpz_clear(m);
      mpz_clear(nprim);
      mpz_clear(n_impair);

  }

  return mpz_get_ui(t);

}



unsigned int verification(mpz_t u, mpz_t z, mpz_t n){

  // VERIFICATION 

    mpz_t verif ;

    mpz_init(verif);

    mpz_mul(verif,z,u);

    mpz_mod(verif,verif,n);

    mpz_clear(verif);

    return mpz_get_ui(verif);

}




















