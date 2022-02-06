#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "chiffrement.h"
#include "dechiffrement.h"
#include "creation_des_cles.h"
#include "miller_rabin.h"

//différentes tailles de bits prédéfinies pour p et q
#define PRIME_LONG_SIZE 1536    //RSA 3072 bits
#define PRIME_MEDIUM_SIZE 1024  //RSA 2048 bits
#define PRIME_LOW_SIZE 512      //RSA 1024 bits
//nombre de bits qui seront utilisés pour p et q
#define PRIME_NUMBER_SIZE 512
 
int main(int argc, char const *argv[])
{    
    mpz_t  m, e, p , q, n, c, phi_n, d;
    mpz_init(m);        //clair
    mpz_init(e);        //exposant publique e : 1 < e < phi(n)
    mpz_init(p);        //nombre premier, premier facteur de n
    mpz_init(q);        //nombre premier, deuxième facteur de n
    mpz_init(n);        //produit des deux nombres premiers
    mpz_init(c);        //chiffré
    mpz_init(phi_n);    //phi(n) : (p - 1) * (q - 1)
    mpz_init(d);        //exposant privé : e^-1 mod ph(n)
    
    srandom(getpid() + time(NULL));

    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) random());
    gmp_printf("\nmessage d'origine : %Zd\n\n", m);

    //génération de n, p, q
    generer_un_nombre_premier(p, PRIME_NUMBER_SIZE);
    generer_un_nombre_premier(q, PRIME_NUMBER_SIZE);
    mpz_mul(n, p, q);
    
    //génération de phi de n
    phi(p, q, phi_n);
    
    //génération de e
    generer_exposant_public(phi_n, e);
    gmp_printf("exposant publique e : %Zd\n", e);

    chiffrement_RSA(m, e, n, c);
    gmp_printf("\nchiffré : %Zd\n", c);
    
    mpz_init(m);    // m = 0
    
    //génération de d
    generer_exposant_privee(e, phi_n, d);

    dechiffrement_RSA(c, d, n, m);
    gmp_printf("\nmessage : %Zd\n", m);

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_init(phi_n);
    mpz_clear(d);

    exit(0);
}