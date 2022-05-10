#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>        //pour faciliter le timing attack (à faire: à retirer lorsque le timing attack sera fini)
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include "miller_rabin.h"
#include "creation_des_cles.h"
#include "square_multiply.h"
#include "chiffrement.h"
#include "temps.h"
#include "timing_attack.h"

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

   random_number[0] = '\0';
   byte[0] = '\0';
   tmp[0] = '\0';

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
   if(padding)
      padding_chiffrement(m);
   square_and_multiply(m, e, n, c);
}

//SHA256 hash
void hash(mpz_t hm)
{
   unsigned int i = 0;
   size_t taille = 0;   
   EVP_MD_CTX* ctx = NULL;          //message digest context
   EVP_MD* sha256 = NULL;           //message digest
   unsigned int error = 1, len = 0;
   unsigned char* outdigest = NULL;
   char* msg = malloc(mpz_sizeinbase(hm, 16) + 1);
   char* tmp = malloc(65);          //32 blocs de SHA256
   char* hexa = malloc(3);
   
   msg[0] = '\0';
   tmp[0] = '\0';
   hexa[0] = '\0';
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
      taille = sizeof(hexa);
      if(strcmp(hexa,"0") == 0)
      {
         strncat(tmp, "00", 2);
      }
      else if((taille == 1) && strcmp(hexa,"0") != 0)
      {
         strncat(tmp, "0", 1);
         strncat(tmp, hexa, 1);
      }
      else
      {
         strncat(tmp, hexa, taille);
      }
      //printf("hexa: %s\n", hexa);
      //printf("hash: %s\n", tmp);
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

   random_number[0] = '\0';
   byte[0] = '\0';
   tmp[0] = '\0';

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
   if(padding)
      padding_signature(m);
   square_and_multiply(m, d, n, s);
}

//--------------------------------------MONTGOMERY-------------------------------------------//

/* Generateur de r =  2^1024 ( RSA 1024 bits ) */
void generer_R_montgomery(mpz_t r)
{
      mpz_ui_pow_ui(r, 2, n_size);
}

/* Applique le theoreme de Bezout sur n et r et renvoie u et v tel que :
*                 - RU + NV = 1
*/
void bezout(const mpz_t r, const mpz_t n , mpz_t u, mpz_t v, mpz_t pgcd)
{
   if(!mpz_cmp_ui(n, 0))
   {
      mpz_set(pgcd, r);
      mpz_set_ui(u, 1);
      mpz_init(v);
   }
   else
   {
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

void Montgomery_product(const mpz_t v, const mpz_t a_bar, const mpz_t b_bar, const mpz_t n, mpz_t t, const unsigned int N_SIZE)
{ 
   mpz_t m, z, nprim; 
   mpz_inits(nprim,m,z,NULL);

   mpz_neg(nprim,v); // nprim = -v
   //MONTGOMERY
   /*
   * m = ( z mod r ) * nprim mod r
   * t = ( z + m * n ) / r
   * si t > n --> t = t - n
   * sinon  t
   */
   mpz_mul(z,a_bar,b_bar); // z = abar bbar
   mpz_mod_2exp(m,z, N_SIZE); // m = ( z mod r )
   mpz_mul(m,m,nprim); //  m = ( z mod r ) * nprim
   mpz_mod_2exp(m,m, N_SIZE); // m = ( z mod r ) * nprim mod r
   mpz_mul(t,m,n ); // t = ( m * n )
   mpz_add(t,z,t); // t = ( z + m * n )
   mpz_tdiv_q_2exp(t,t, N_SIZE); // t = ( z + m * n ) / r

   //###########################-TIMING ATTACK-#################################//
   double tta = 0.0, tta_cpu = 0.0;
   clock_t tta_cpu_deb = 0, tta_cpu_fin = 0;
   struct timespec tta_deb = {0,0}, tta_fin = {0,0};

   //if(bit_cible == bit_position)
   if(TIMING_ATTACK_CONFIRMED)
      debut_chrono(&tta_cpu_deb,&tta_deb);
      //debut_chrono_timing_attack(&tta_deb);
   //###########################################################################//
   
   //int in_if = 0;
   
   if((mpz_cmp(t, n) == 0) || (mpz_cmp(t,n) > 0))
   {
      mpz_sub(t, t, n); // t = t - n

      //in_if = 1;
   }
   //printf("bit_position = %u\n", bit_position);
   //###########################-TIMING ATTACK-#################################//
   //if(bit_cible == bit_position)
   //{
   if(TIMING_ATTACK_CONFIRMED)
   {
      /*if(in_if)
         printf("bit %d\n", bit_position);*/
      fin_chrono(&tta_cpu,tta_cpu_deb,tta_cpu_fin,&tta,tta_deb,tta_fin);
      //printf("\n");
      
      //fin_chrono_timing_attack(&tta, tta_deb, tta_fin);
      
      ELEMENT* elem = initialiser_element(tta_cpu);
      
      /*if(in_if)
         afficher_element(elem, "elem");*/
      if(elem->temps >= 0.000002)
         ajouter_element_global(elem, &A, bit_position);
      else
         ajouter_element_global(elem, &B, bit_position);
   }
   //}
   //###########################################################################//

   mpz_clear(m);
   mpz_clear(nprim);
   mpz_clear(z);
}

void Montgomery_Exponentiation_crypt(mpz_t crypt, const mpz_t a, const mpz_t v, const mpz_t e, const mpz_t n, const unsigned int N_SIZE)
{
   unsigned int k, taille;
   taille = mpz_sizeinbase(e, 2);
   mpz_t a_bar, x_bar, rop1, un, rshiftr, andr , msk;
   mpz_inits(a_bar, x_bar, rop1, un, rshiftr, andr, msk, NULL);
   mpz_set_ui(un, 1);
   mpz_set_ui(msk, 1);

   mpz_mul_2exp(rop1, a, N_SIZE); // rop1 = a * r (r = 2^N_SIZE)
   mpz_mod(a_bar, rop1, n); // a_bar = ( a * r ) mod n
   mpz_mul_2exp(x_bar, un, N_SIZE); // x_bar = 1 * r (r = 2^N_SIZE) 

   bit_cible = n_size;
   for(k = taille; k > 0; k--)
   {
   //###########################-TIMING ATTACK-#################################//
      if(taille == n_size)
         bit_position = k;
      if(taille == n_size-1)
      {
         bit_position = k+1;
         if(bit_cible < 2)
            bit_position = bit_cible;
      } 
      if(taille == n_size-2)
      {
         bit_position = k+2;
         if(bit_position < 3)
            bit_position = bit_cible;

         printf("k : %u\n", k);
         printf("bit pos : %u ", bit_position);
         printf("bit cible : %u\n\n\n", bit_cible);
      }
   //###########################################################################//

      Montgomery_product(v, x_bar, x_bar, n, x_bar, N_SIZE); // square 
      mpz_tdiv_q_2exp(rshiftr, e, k - 1);
      mpz_and(andr, rshiftr, msk);

      TIMING_ATTACK_CONFIRMED = 1;  //active le timing attack

      if(!(mpz_cmp_ui(andr, 1)))
      {  
         Montgomery_product(v, a_bar, x_bar, n, x_bar, N_SIZE); // multiply 
      }

      TIMING_ATTACK_CONFIRMED = 0;  //active le timing attack
      bit_cible--;
   }
   Montgomery_product(v, x_bar, un, n, crypt, N_SIZE); // calcul du chiffre

   mpz_clears(a_bar, x_bar, rop1, un, rshiftr, andr, msk, NULL);
}

void verification(const mpz_t u, const mpz_t z, const mpz_t n, mpz_t verif)
{
   mpz_mul(verif, z, u); mpz_mod(verif, verif, n);
}

void chiffrement_RSA_montgomery(mpz_t m, const mpz_t e, const mpz_t n, mpz_t c, const mpz_t v, const unsigned int N_SIZE)
{
   if(padding)
      padding_chiffrement(m);
   Montgomery_Exponentiation_crypt(c, m, v, e, n, N_SIZE);
}
