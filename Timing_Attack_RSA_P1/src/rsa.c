#define _GNU_SOURCE //fix warning implicit declaration

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <gmp.h>
#include "temps.h"
#include <time.h>
#include <sys/time.h>
#include "chiffrement.h"
#include "dechiffrement.h"
#include "creation_des_cles.h"
#include "rsa.h"

static void choix_mode_rsa(char* choix)
{
    printf("\n\t Choisir le mode de RSA : \n\t - Pour le mode Classique tapez 1 \n\t - Pour le mode Montgomery tapez 2\n");
    *choix = getchar();
    while(getchar() != '\n');
 
    printf("value : %c\n", *choix);

    if(*choix != '1' && *choix != '2')
    {
        choix_mode_rsa(choix);
    }
}

void run()
{
    char c = '\0';
    choix_mode_rsa(&c);

    if(c == '1')
    {
        run_rsa();
    }
    if(c == '2')
    {
        run_rsa_montgomery();
    }
}

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

    srand(getpid() + time(NULL)); //utiliser pour p, q, PKCS#1, (et le message clair)

    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) rand());
    gmp_printf("\nmessage d'origine : %Z0X\n", m);
    mpz_set(s, m);
    mpz_set(ms, m);

/*---------------------------------------CHRONO-------------------------------------------------*/
/**/clock_t t_cpu_deb = 0, t_cpu_fin = 0;                                                       //
/**/struct timespec t_reel_deb = {0,0}, t_reel_fin = {0,0};                                     //
/**/double temps_reel_total = 0.0, temps_cpu_total = 0.0;                                       //
/**/debut_chrono(&t_cpu_deb,&t_reel_deb);                                                       //
/*----------------------------------------------------------------------------------------------*/

    //génération de n, p, q
    generer_npq(n, p, q);
    //gmp_printf("\np : %Zd, \nq : %Zd, \nn : %Zd\n", p, q, n);
    
    //génération de phi(n)
    phi(p, q, phi_n);
    //gmp_printf("\nphi(n) : %Zd\n", phi_n);
    
    //génération de e
    generer_exposant_public(e);
    //gmp_printf("\nexposant publique e : %Zd\n", e);

    //génération de d
    generer_exposant_privee(e, phi_n, d);
    //gmp_printf("\nexposant privee d : %Zd\n", d);

    //chiffrement de m
    chiffrement_RSA(m, e, n, c);
    gmp_printf("\nchiffré : %Z0X\n", c);

    //signature de m
    signature(ms, d, n, s);
    //gmp_printf("\nsignature : %Z0X\n", s);

    //vérification de s
    verification_signature(s, e, n, ms);

    //dechifrement de m
    dechiffrement_RSA(c, d, n, m);
    gmp_printf("\nmessage déchiffré : %Z0X\n", m);
/*--------------------------------------FIN_CHRONO----------------------------------------------*/
/**/fin_chrono(&temps_cpu_total,t_cpu_deb,t_cpu_fin,&temps_reel_total,t_reel_deb,t_reel_fin);   //
/*----------------------------------------------------------------------------------------------*/

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_clear(phi_n);
    mpz_clear(d);
    mpz_clear(s);
    mpz_clear(ms);
}

void run_rsa_montgomery()
{
    mpz_t  m, e, p , q, n, c, phi_n, d, s, ms, r, u, v, pgcd_bezout;
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
    mpz_init(r);
    mpz_init(u);
    mpz_init(v);
    mpz_init(pgcd_bezout);

    srand(getpid() + time(NULL)); //utiliser pour p, q, PKCS#1, (et le message clair)

    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) rand());
    gmp_printf("\nmessage d'origine : %Z0X\n", m);
    mpz_set(s, m);
    mpz_set(ms, m);

/*---------------------------------------CHRONO-------------------------------------------------*/
/**/clock_t t_cpu_deb = 0, t_cpu_fin = 0;                                                       //
/**/struct timespec t_reel_deb = {0,0}, t_reel_fin = {0,0};                                     //
/**/double temps_reel_total = 0.0, temps_cpu_total = 0.0;                                       //
/**/debut_chrono(&t_cpu_deb,&t_reel_deb);                                                       //
/*----------------------------------------------------------------------------------------------*/

    //génération de n, p, q
    generer_npq(n, p, q);
    //gmp_printf("\np : %Zd, \nq : %Zd, \nn : %Zd\n", p, q, n);
    
    //génération de phi(n)
    phi(p, q, phi_n);
    //gmp_printf("\nphi(n) : %Zd\n", phi_n);
    
    //génération de e
    generer_exposant_public(e);
    //gmp_printf("\nexposant publique e : %Zd\n", e);

    //génération de d
    generer_exposant_privee(e, phi_n, d);
    //gmp_printf("\nexposant privee d : %Zd\n", d);

    //générer R
    generer_R_montgomery(r,n);

    bezout(r, n, u, v, pgcd_bezout);
    //gmp_printf("R(%Zd) * v(%Zd) + N(%Zd) * u(%Zd)  \n\n", r, u, n, v);

    mp_bitcnt_t  N_SIZE = (mp_bitcnt_t)mpz_sizeinbase(n, 2); //nombre de bits de n 

    chiffrement_RSA_montgomery(m, e, n, c, v,N_SIZE);
    gmp_printf("\nchiffré : %Z0X\n", c);

    //signature de m
    signature(ms, d, n, s);
    //gmp_printf("\nsignature : %Z0X\n", s);

    //vérification de s
    verification_signature(s, e, n, ms);

    dechiffrement_RSA_montgomery(c, d, n, m, v,N_SIZE);
    gmp_printf("\nmessage déchiffré : %Z0X\n", m);

/*--------------------------------------FIN_CHRONO----------------------------------------------*/
/**/fin_chrono(&temps_cpu_total,t_cpu_deb,t_cpu_fin,&temps_reel_total,t_reel_deb,t_reel_fin);   //
/*----------------------------------------------------------------------------------------------*/

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_clear(phi_n);
    mpz_clear(d);
    mpz_clear(s);
    mpz_clear(ms);
    mpz_clear(r);
    mpz_clear(u);
    mpz_clear(v);
    mpz_clear(pgcd_bezout);
}
