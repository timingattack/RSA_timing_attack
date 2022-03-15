#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "chiffrement.h"
#include "dechiffrement.h"
#include "square_multiply.h"

//PKCS#1 v1.5 pour le déchiffrement (extraie m de PKCS#1 chiffré)
static void padding_dechiffrement(mpz_t m)
{
   size_t i;
   size_t size_m = mpz_sizeinbase(m, 2);
   char* str_m = malloc(mpz_sizeinbase(m, 2) + 1);
   mpz_t msk;
   mpz_init(msk);

   strncpy(str_m, "\0", strlen(str_m));
   
   for(i = 0; i < size_m - (size_m - 74); i++)
   {
      strncat(str_m, "0", 1);
   }

   for(i = 0; i < size_m - 74; i++)
   {
      strncat(str_m, "1", 1);
   }

   mpz_set_str(msk, str_m, 2);
   mpz_and(m, m, msk);

   mpz_clear(msk);
   free(str_m);
}

//PKCS#1 v1.5 pour la vérification de la signature (extraire m de PKCS#1 signé)
static void padding_verification_signature(mpz_t m)
{
   size_t i;
   size_t size_m = mpz_sizeinbase(m, 2);
   char* str_m = malloc(mpz_sizeinbase(m, 2) + 1);
   mpz_t msk;
   mpz_init(msk);

   strncpy(str_m, "\0", strlen(str_m));
   
   for(i = 0; i < size_m - (size_m - 73); i++)
   {
      strncat(str_m, "0", 1);
   }

   for(i = 0; i < size_m - 73; i++)
   {
      strncat(str_m, "1", 1);
   }

   mpz_set_str(msk, str_m, 2);
   mpz_and(m, m, msk);

   mpz_clear(msk);
   free(str_m);
}

void dechiffrement_RSA(const mpz_t c, const mpz_t d, const mpz_t n, mpz_t m)
{
   square_and_multiply(c, d, n, m);
   padding_dechiffrement(m);
}

//vérifie la signature s^(e) avec m
void verification_signature(mpz_t s, const mpz_t e, const mpz_t n, const mpz_t m)
{
   mpz_t tmp_s;
   mpz_init(tmp_s);
   mpz_set(tmp_s, s);
   padding_verification_signature(tmp_s);
   square_and_multiply(s, e, n, tmp_s);

   if(!(mpz_cmp(tmp_s, m)))
   {
      printf("la signature est valide.\n");
   } else {   
      fprintf(stderr, "la signature est invalide.\n");

      mpz_clear(tmp_s);

      exit(8);
   }

   mpz_clear(tmp_s);
}

//--------------------------------------MONTGOMERY-------------------------------------------//


/*void Montgomery_Exponentiation_decrypt(mpz_t decrypt, const mpz_t a, const mpz_t v, const mpz_t d, const mpz_t r, const mpz_t n)
{
   Montgomery_Exponentiation_crypt(decrypt ,a ,v ,d ,r ,n);
}


void dechiffrement_RSA_montgomery(const mpz_t c, const mpz_t d, const mpz_t n, mpz_t m, const mpz_t v, const mpz_t r)
{
   Montgomery_Exponentiation_decrypt(m,c,v,d,r,n);
   padding_dechiffrement(m);
}
*/

void Montgomery_Exponentiation_decrypt(mpz_t decrypt, const mpz_t a, const mpz_t v, const mpz_t d, const mpz_t n, unsigned int N_SIZE)
{
   Montgomery_Exponentiation_crypt(decrypt ,a ,v ,d ,n,N_SIZE);
}


void dechiffrement_RSA_montgomery(const mpz_t c, const mpz_t d, const mpz_t n, mpz_t m, const mpz_t v,unsigned int N_SIZE)
{
   Montgomery_Exponentiation_decrypt(m,c,v,d,n,N_SIZE);
   padding_dechiffrement(m);
}
