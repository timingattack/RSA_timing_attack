#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "creation_des_cles.h"
#include "square_multiply.h"
#include "miller_rabin.h"

//différentes tailles de bits prédéfinies pour p et q
#define PRIME_LONG_SIZE 1536    //RSA 3072 bits
#define PRIME_MEDIUM_SIZE 1024  //RSA 2048 bits
#define PRIME_LOW_SIZE 512      //RSA 1024 bits
//nombre de bits qui seront utilisés pour p et q
#define PRIME_NUMBER_SIZE PRIME_LONG_SIZE

unsigned int pgcd(unsigned int a, unsigned int b)
{
    if (b != 0)
       return pgcd(b, a%b);
    else 
       return a;
}

void generer_npq(mpz_t n, mpz_t p, mpz_t q)
{
    if(PRIME_NUMBER_SIZE < 3)
    {
       generer_un_nombre_premier(p, 3);
       generer_un_nombre_premier(q, 3);
    } else {
        generer_un_nombre_premier(p, PRIME_NUMBER_SIZE); 
        generer_un_nombre_premier(q, PRIME_NUMBER_SIZE);
    }

    if(mpz_cmp(p, q))
    {
        mpz_mul(n, p, q);
        
        return;     
    } else {
        mpz_init(n);
        mpz_init(p);
        mpz_init(q);

        generer_npq(n, p, q);

        return;
    }
}

void generer_exposant_public(const mpz_t phi_n, mpz_t e)
{
   mpz_t random_value, pgcd_r;
   mpz_init(random_value);
   mpz_init(pgcd_r); //résultat de PGCD(e, phi(n))

   //génère un 1 < e < phi(n)
   while(mpz_cmp_ui(e, 2) < 0)
   {
      mpz_init(e);
      mpz_set_ui(random_value, (unsigned long int) random());
      mpz_mod(e, random_value, phi_n);
   }

   //si PGCD(e, phi(n)) = 1
   mpz_gcd(pgcd_r, e, phi_n);
   if(!(mpz_cmp_ui(pgcd_r, 1)))
   {
      mpz_clear(random_value);
      mpz_clear(pgcd_r);

      return;  //on sort car e est valide pour RSA
   } else {
      mpz_init(e);

      mpz_clear(random_value);
      mpz_clear(pgcd_r);

      generer_exposant_public(phi_n, e); //sinon on génère un autre e
   }
}

void phi(const mpz_t p, const mpz_t q, mpz_t phi)
{
   mpz_t tmp_p, tmp_q;
   mpz_init(tmp_p);
   mpz_init(tmp_q);

   //p - 1
   mpz_sub_ui(tmp_p, p, 1);

   //q - 1
   mpz_sub_ui(tmp_q, q, 1);

   //phi = (p - 1) * (q - 1)
   mpz_mul(phi, tmp_p, tmp_q);

   mpz_clear(tmp_p);
   mpz_clear(tmp_q);
}


void generer_exposant_privee(const mpz_t e, const mpz_t phi_n, mpz_t d)
{
   mpz_t pgcd_r;
   mpz_init(pgcd_r);

   //si l'inverse modulaire n'existe pas on arrête le programme
   if(!(mpz_invert(d, e, phi_n)))
   {
       mpz_init(d);
        
       mpz_clear(pgcd_r);

       fprintf(stderr,"Erreur: l'inverse modulaire de l'exposant public n'existe pas.\n");
    
       exit(5);
   } else {
       mpz_gcd(pgcd_r, e, d);
       
       //si le PGCD(e,d) ≠ 1 on arrête le programme
       if(mpz_cmp_ui(pgcd_r, 1))
       {
           mpz_init(d);
        
           mpz_clear(pgcd_r);

           fprintf(stderr,"Erreur: l'inverse modulaire de l'exposant public n'est pas premier avec l'exposant privé d.\n");

           exit(6);
       }
   }
}
