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

int main()
{   
    mpz_t  m, e, p , q, n, c, phi_n, d, ps,ss;
    mpz_init(m);        //clair
    mpz_init(e);        //exposant publique e : 1 < e < phi(n)
    mpz_init(p);        //nombre premier, premier facteur de n
    mpz_init(q);        //nombre premier, deuxième facteur de n
    mpz_init(n);        //produit des deux nombres premiers
    mpz_init(c);        //chiffré
    mpz_init(phi_n);    //phi(n) : (p - 1) * (q - 1)
    mpz_init(d);        //exposant privé : e^-1 mod ph(n)
    mpz_init(ss);       //signature : µ(m)^(d) mod n
    mpz_init(ps);       //PKCS#1 signature : µ(m)

    srandom(getpid() + time(NULL)); //utiliser pour p, q, PKCS#1, (et le message clair)

    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) random());
    gmp_printf("\nmessage d'origine : %Z0X\n", m);
    mpz_set(ss, m);
    mpz_set(ps, m);

    //génération de n, p, q
    generer_npq(n, p, q);
    //gmp_printf("\np : %Zd, q : %Zd, n : %Zd\n", p, q, n);
    
    //génération de phi(n)
    phi(p, q, phi_n);
    //gmp_printf("\nphi(n) : %Zd\n", phi_n);
    
    //génération de e
    generer_exposant_public(phi_n, e);
    gmp_printf("\nexposant publique e : %Zd\n", e);

    //chiffrement de m
    chiffrement_RSA(m, e, n, c);
    //gmp_printf("\nchiffré : %Z0X\n", c);

    //génération de d
    generer_exposant_privee(e, phi_n, d);
    //gmp_printf("\nexposant privee d : %Zd\n", d);

    //signature de m
    signature(ps, d, n, ss);
    gmp_printf("\nsignature : %Z0X\n", ss);

    dechiffrement_RSA(c, d, n, m);
    gmp_printf("\nmessage déchiffré : %Z0X\n", m);

    verification_signature(ss, e, n, ps);

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_init(phi_n);
    mpz_clear(d);
    mpz_clear(ss);
    mpz_clear(ps);

    exit(0);
}
