#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <stdbool.h>
#include <math.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include "chiffrement.h"
#include "square_multiply.h"
#include "miller_rabin.h"

#define PADDING_SIZE 88
#define BYTE_SIZE 8

/* PKCS#1 v1.5 pour le chiffrement
* - de la forme : 00 02 ALEA 00 | M
* - taille :        (88 bits)   | (variable)
*/
static void padding_chiffrement(mpz_t m)
{
   size_t i, j;
   double bit_value = 0, limit_value = 0;
   char* random_number = malloc(sizeof(char) * PADDING_SIZE + mpz_sizeinbase(m, 2) + 1); //chaine de 88 bits (11 octets) + bits du message
   char* byte = malloc(sizeof(char) * BYTE_SIZE + 1);   //chaine de 8 bits (1 octet)
   char* tmp = malloc(mpz_sizeinbase(m,2) + 1);
   
   if(!random_number || !byte || !tmp)
   {
      fprintf(stderr, "Erreur: échec du malloc pour le padding chiffrement.\n");
      free(random_number);
      free(byte);
      free(tmp);
      exit(7);
   }

   strncpy(random_number,"\0", strlen(random_number));
   strncpy(byte,"\0", strlen(byte));
   strncpy(tmp,"\0", strlen(tmp));

   for(i = 0; i < 12; i++)
   {
      //message
      if(i == 11)
      {
         mpz_get_str(tmp, 2, m);
         size_t taille_message = strlen(tmp);

         for(j = 0; j <= taille_message; j++)
         {
            strncat(random_number, &tmp[j], 1);
         }

         mpz_set_str(m, random_number, 2);
         //gmp_printf("PKCS#1 v1.5 chiffré: %Z0X\n", m);
         break;
      }
      //1 octet ØØ
      if(i == 10)
      {
         for(j = 0; j < BYTE_SIZE; j++)
         {  
            strncat(random_number, "0", 1);
         }
         continue;
      }
      //8 octets aléatoires différents de 00
      if(1 < i && i < 10)
      {
         do 
         {
            strncpy(byte,"\0", strlen(byte));
            for(j = 0; j < BYTE_SIZE; j++)
            {
               random_bit(&byte, bit_value, limit_value);
            }
         } while(strncmp(byte,"00000000", BYTE_SIZE) == 0);

         strncat(random_number, byte, BYTE_SIZE);
         continue;
      }
      //#1 octet Ø2
      if(i == 1)
      {
         strncat(random_number, "00000010", BYTE_SIZE);
         continue;
      }
      //#1 octet ØØ
      if(i == 0)
         strncat(random_number, "00000000", BYTE_SIZE);
   }

   free(random_number);
   free(byte);
   free(tmp);
}

void chiffrement_RSA(mpz_t m, const mpz_t e, const mpz_t n, mpz_t c)
{
   padding_chiffrement(m);
   square_and_multiply(m, e, n, c);
}

//SHA256 hash
void hash(mpz_t hm)
{
   int i = 0;
   size_t taille = 0;   
   EVP_MD_CTX* ctx = NULL;          //message digest context
   EVP_MD* sha256 = NULL;           //message digest
   unsigned int error = 1, len = 0;
   unsigned char* outdigest = NULL;
   char* msg = malloc(mpz_sizeinbase(hm, 16) + 1);
   char* tmp = malloc(65);          //32 blocs de SHA256
   char* hexa = malloc(3);
   
   strncpy(msg,"\0", strlen(msg));
   strncpy(tmp,"\0", strlen(tmp));
   strncpy(hexa,"\0", strlen(hexa));
   mpz_get_str(msg, 16, hm);        //recupération du message en hexa

   // Create a context for the digest operation
   ctx = EVP_MD_CTX_new();
   if (ctx == NULL)
      goto err;

   /*
   * Fetch the SHA256 algorithm implementation for doing the digest. We're
   * using the "default" library context here (first NULL parameter), and
   * we're not supplying any particular search criteria for our SHA256
   * implementation (second NULL parameter). Any SHA256 implementation will
   * do.
   */
   sha256 = EVP_MD_fetch(NULL, "SHA256", NULL);
   if(sha256 == NULL)
      goto err;

   // Initialise the digest operation
   if(!EVP_DigestInit_ex(ctx, sha256, NULL))
      goto err;

   /*
   * Pass the message to be digested. This can be passed in over multiple
   * EVP_DigestUpdate calls if necessary
   */
   if(!EVP_DigestUpdate(ctx, msg, sizeof(msg)))
      goto err;

   // Allocate the output buffer
   outdigest = OPENSSL_malloc(EVP_MD_get_size(sha256));
   if(outdigest == NULL)
      goto err;

   // Now calculate the digest itself
   if(!EVP_DigestFinal_ex(ctx, outdigest, &len))
      goto err;

   // Print out the digest result
   //BIO_dump_fp(stdout, outdigest, len);
   
   for(i = 0; i < 32; i++)
   {
      sprintf(hexa, "%x", outdigest[i]);
      taille = strlen(hexa);
      if(strcmp(hexa,"0") == 0)
      {
         i+=2;
         strncat(tmp, "00", 2);
      }
      else if((taille == 1) && strcmp(hexa,"0") != 0)
      {
         i++;
         strncat(tmp, "0", 1);
         strncat(tmp, hexa, 1);
      }
      else
      {
         strncat(tmp, hexa, taille);
      }
   }
   mpz_set_str(hm, tmp, 16);   //récupération du hash

   error = 0;

   err:
      // Clean up all the resources we allocated
      OPENSSL_free(outdigest);
      EVP_MD_free(sha256);
      EVP_MD_CTX_free(ctx);
      free(msg);
      free(tmp);
      free(hexa);
      if(error != 0)
         ERR_print_errors_fp(stderr);
}

/* PKCS#1 v1.5 pour la signature
* - de la forme : 00 01 FF FF FF FF FF FF FF FF 00 | H(M)
* - taille :                  (88 bits)            | (variable)
*/
void padding_signature(mpz_t m)    //m est déjà hashé
{
   size_t i, j;
   char* random_number = malloc(sizeof(char) * PADDING_SIZE + mpz_sizeinbase(m, 2) + 1); //chaine de 88 bits (11 octets) + bits du message
   char* byte = malloc(sizeof(char) * BYTE_SIZE + 1);   //1 octet
   char* tmp = malloc(mpz_sizeinbase(m, 2) + 1);
   
   if(!random_number || !byte || !tmp)
   {
      fprintf(stderr, "Erreur: échec du malloc pour le padding signature.\n");
      free(random_number);
      free(byte);
      free(tmp);
      exit(9);
   }

   strncpy(random_number,"\0", strlen(random_number));
   strncpy(byte,"\0", strlen(byte));
   strncpy(tmp,"\0", strlen(tmp));

   for(i = 0; i < 12; i++)
   {
      //message
      if(i == 11)
      {
         mpz_get_str(tmp, 16, m);       
         size_t taille_message = strlen(tmp);

         for(j = 0; j <= taille_message; j++)
         {
            strncat(random_number, &tmp[j], 1);
         }

         mpz_set_str(m, random_number, 16);

         //gmp_printf("PKCS#1 v1.5 signé: %Z0X\n", m);
         break;
      }
      //1 octet ØØ
      if(i == 10)
      {
         strncat(random_number, "00", 2);
         continue;
      }
      //8 octets FF
      if(1 < i && i < 10)
      {
         strncat(random_number, "FF", 2);
         continue;
      }
      //#1 octet Ø1
      if(i == 1)
      {
         strncat(random_number, "01", 2);
         continue;
      }
      //#1 octet ØØ
      if(i == 0)
         strncat(random_number, "00", 2);
   }

   free(random_number);
   free(byte);
   free(tmp);
}

// signe le message lors du chiffrement
void signature(mpz_t m, const mpz_t d, const mpz_t n, mpz_t s)
{
   padding_signature(m);
   square_and_multiply(m, d, n, s);
}

//--------------------------------------MONTGOMERY-------------------------------------------//

void generer_R_montgomery(mpz_t r, const mpz_t n)
{
   unsigned int i = 0;
   while(mpz_cmp(r, n) < 0)
   {
      mpz_ui_pow_ui(r, 2, i++);
   }
}

/* Applique le theoreme de Bezout sur n et r et renvoie u et v tel que :
* - RU + NV = 1
*/
void bezout(const mpz_t r, const mpz_t n , mpz_t u, mpz_t v, mpz_t pgcd)
{
   if(!mpz_cmp_ui(n, 0))
   {
      mpz_set(pgcd, r);
      mpz_set_ui(u, 1);
      mpz_init(v);
   }
   else {
      mpz_t q, reste, u1, v1, v_q;
      mpz_inits(q, reste, u1, v1, v_q, NULL);
      
      mpz_tdiv_qr(q, reste, r, n);
      bezout(n, reste, u1, v1, pgcd); 
      mpz_set(u, v1);
      mpz_mul(v_q, v1, q);
      mpz_sub(v, u1, v_q);

      mpz_clears(q, reste, u1, v1, v_q, NULL);
   }
}

void Montgomery_product(const mpz_t v, const mpz_t a_bar, const mpz_t b_bar, const mpz_t r, const mpz_t n, mpz_t res)
{
   mpz_t t, m, nprim, rop1, rop2, rop3;
   mpz_inits(t, m, nprim, rop1, rop2, rop3, NULL);
 
   mpz_neg(nprim, v); // nprim = -v
  
   mpz_mul(rop1, a_bar, b_bar);
   mpz_mod(t, rop1, r);
   mpz_mul(m, t, nprim);
   mpz_mod(m, m, r);
   mpz_mul(rop2, m,n);
   mpz_add(rop3, rop1, rop2);
   mpz_init(res);
   mpz_div(res, rop3, r);

   if((mpz_cmp(res, n) == 0) || (mpz_cmp(res,n) > 0))
   {
      mpz_sub(res, res, n);
   }

   mpz_clears(t, m, nprim, rop1, rop2, rop3, NULL);
}

void Multiplication_modulaire_montgomery(const mpz_t v, const mpz_t a, const mpz_t b, const mpz_t r, const mpz_t n, mpz_t res)
{
   mpz_t x_bar, x, a_bar, b_bar, rop1, rop2 , un;
   mpz_inits(x, x_bar, a_bar, b_bar, rop1, rop2, un, NULL);
   mpz_set_ui(un, 1);

   mpz_mul(rop1, a,r);
   mpz_mod(a_bar, rop1, n);
   mpz_mul(rop2, b, r);
   mpz_mod(b_bar, rop2, n);
   Montgomery_product(v, a_bar, b_bar, r, n, x_bar);
   Montgomery_product(v, x_bar, un, r, n, x);

   mpz_set(res, x);

   mpz_clears(x, x_bar, a_bar, b_bar, rop1, rop2, un, NULL);
}


void Montgomery_Exponentiation_crypt(mpz_t crypt , const mpz_t a, const mpz_t v, const mpz_t e , const mpz_t r, const mpz_t n)
{
   unsigned int k, taille;
   taille = mpz_sizeinbase(e, 2);
   mpz_t a_bar, x_bar, rop1, rop2, un, rshiftr, andr , msk;
   mpz_inits(a_bar, x_bar, rop1, rop2, un, rshiftr, andr, msk, NULL);
   mpz_set_ui(un, 1);
   mpz_set_ui(msk, 1);

   mpz_mul(rop1, a, r);
   mpz_mod(a_bar, rop1, n);
   mpz_mul(x_bar, un, r);

   for(k = taille; k > 0; k--)
   {
      Montgomery_product(v, x_bar, x_bar, r, n, x_bar);

      mpz_tdiv_q_2exp(rshiftr, e, k - 1);
      mpz_and(andr, rshiftr, msk);
      if(!(mpz_cmp_ui(andr, 1)))
      {    
         Montgomery_product(v, a_bar, x_bar, r, n, x_bar);
      }
   }

   Montgomery_product(v, x_bar, un, r, n, crypt);

   mpz_clears(a_bar, x_bar, rop1, rop2, un, rshiftr, andr, msk, NULL);
}


void verification(const mpz_t u, const mpz_t z, const mpz_t n, mpz_t verif)
{
   mpz_mul(verif, z, u); mpz_mod(verif, verif, n);
}

void chiffrement_RSA_montgomery(mpz_t m, const mpz_t e, const mpz_t n, mpz_t c, const mpz_t v, const mpz_t r)
{
   padding_chiffrement(m);
   Montgomery_Exponentiation_crypt(c, m, v, e, r, n);
}
