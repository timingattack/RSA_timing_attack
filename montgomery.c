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
	// Si a ou b = 0 

                  /* --- RU + NV = 1 */

  if (!mpz_cmp_ui(n, 0))
    {
    	printf("------------ THEOREME DE BEZOUT -------------\n"); 

      mpz_set(pgcd, r);

      mpz_set_ui(u, 1);

      mpz_set(v, x);

      gmp_printf(" %Zd * ( %Zd ) + %Zd * ( %Zd ) = %Zd \n",r,u,n,v,pgcd);


        
    }

    // sinon 

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

      gmp_printf(" %Zd * ( %Zd ) + %Zd * ( %Zd ) = %Zd \n",r,u,n,v,pgcd);


      printf("\n");
      mpz_clear(q);
      mpz_clear(reste);
      mpz_clear(u1);
      mpz_clear(v1);
      mpz_clear(v_q);
    }

   

}


void montgomery_essai(mpz_t u, mpz_t v, mpz_t z, mpz_t r, mpz_t n,mpz_t pgcd){


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
    gmp_printf("r et n ne sont pas premier pgcd = %Zd\n", pgcd);
  }

  if (mpz_cmp(r,n) <= 0)
  {
    gmp_printf("r est inferieur ou egal a n\n  r = %Zd et n = %Zd\n", r,n);
  }

  if (mpz_mod_ui(n_impair, n , 2) == 0)
  {
    gmp_printf(" n est pair n = %Zd \n",n);
    
  }

  if (mpz_cmp_ui(n,3) <= 0)
  {
     gmp_printf(" n est inferieur a 3 n = %Zd \n",n);
    
  }

  if (mpz_cmp(r,n) > 0 && mpz_get_ui(pgcd) == 1 && mpz_mod_ui(n_impair, n , 2) != 0 && mpz_cmp_ui(n,3) > 0)
  {
     mpz_mod(m,z,r); // m = ( z mod r )

    gmp_printf("m = %Zd \n",m);

    mpz_mul(m, m,nprim ); //  m = ( z mod r ) * nprim 

    gmp_printf("m = %Zd \n",m);

    mpz_mod(m,m,r); // m = ( z mod r ) * nprim mod r

    gmp_printf("m = %Zd \n",m);

    mpz_mul(t, m,n ); // t = ( m * n )

    gmp_printf("t = %Zd \n",t);

    mpz_add(t,z,t); // t = ( z + m * n )

    gmp_printf("t = %Zd \n",t);

    mpz_div(t,t,r); // t = ( z + m * n ) / r

    if (t > n)
    {
      mpz_sub(t,t,n); // t > n --> t = t-n 

      gmp_printf("t = %Zd \n",t);

    }
    else
      
      {gmp_printf("t = %Zd \n",t);}


    // VERIFICATION 

    mpz_t verif ;

    mpz_init(verif);

    mpz_mul(verif,z,u);

    mpz_mod(verif,verif,n);

    gmp_printf("verif = %Zd \n",verif);

      mpz_clear(verif);
      mpz_clear(m);
      mpz_clear(nprim);
      mpz_clear(n_impair);

  }

 

   

}