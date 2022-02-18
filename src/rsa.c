#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "chiffrement.h"
#include "dechiffrement.h"
#include "creation_des_cles.h"

void run_rsa()
{   
    mpz_t  m, e, p , q, n, c, phi_n, d, s, ms;
    mpz_init(m);        //clair
    mpz_init(e);        //exposant publique e : 1 < e < phi(n)
    mpz_init(p);        //nombre premier, premier facteur de n
    mpz_init(q);        //nombre premier, deuxième facteur de n
    mpz_init(n);        //produit des deux nombres premiers
    mpz_init(c);        //chiffré
    mpz_init(phi_n);    //phi(n) : (p - 1) * (q - 1)
    mpz_init(d);        //exposant privé : e^-1 mod ph(n)
    mpz_init(s);        //signature : µ(m)^(d) mod n
    mpz_init(ms);       //PKCS#1 signature : µ(m)

    srandom(getpid() + time(NULL)); //utiliser pour p, q, PKCS#1, (et le message clair)

    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) random());
    gmp_printf("\nmessage d'origine : %Z0X\n", m);
    mpz_set(s, m);
    mpz_set(ms, m);

    //génération de n, p, q
    generer_npq(n, p, q);
    //gmp_printf("\np : %Zd, \nq : %Zd, \nn : %Zd\n", p, q, n);
    
    //génération de phi(n)
    phi(p, q, phi_n);
    //gmp_printf("\nphi(n) : %Zd\n", phi_n);
    
    //génération de e
    generer_exposant_public(e);
    gmp_printf("\nexposant publique e : %Zd\n", e);

    //génération de d
    generer_exposant_privee(e, phi_n, d);
    //gmp_printf("\nexposant privee d : %Zd\n", d);

    //chiffrement de m
    chiffrement_RSA(m, e, n, c);
    //gmp_printf("\nchiffré : %Z0X\n", c);

    //signature de m
    signature(ms, d, n, s);
    //gmp_printf("\nsignature : %Z0X\n", s);

    //vérification de s
    verification_signature(s, e, n, ms);

    //dechifrement de m
    dechiffrement_RSA(c, d, n, m);
    gmp_printf("\nmessage déchiffré : %Z0X\n", m);

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_init(phi_n);
    mpz_clear(d);
    mpz_clear(s);
    mpz_clear(ms);
}
