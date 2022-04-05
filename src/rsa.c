#define _GNU_SOURCE //fix timespec initialisation

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
    int ctr = 1;
    printf("\n\tChoisir le mode de RSA :\n\t- normal (1)\n\t- montgomery (2)\n\t  ");
    *choix = getchar();
    while(getchar() != '\n')
        ctr++;

    if((*choix != '1' && *choix != '2') || ctr != 1)
    {
        fprintf(stderr, "Erreur : saisie incorrecte\n");
        printf("Usage : entrez la touche 1 ou 2\n");
        choix_mode_rsa(choix);
    }
}

static void choix_taille_module_rsa(char* choix)
{
    int ctr = 1;
    printf("\n\tChoisir la taille du module RSA :\n\t- petite : 1024 (1)\n\t- moyenne : 2048 (2)\n\t- grande : 3072 (3)\n\t- très grande : 4096 (4)\n\t  ");
    *choix = getchar();
    while(getchar() != '\n')
        ctr++;

    if((*choix != '1' && *choix != '2' && *choix != '3' && *choix != '4') || ctr != 1)
    {
        fprintf(stderr, "Erreur : saisie incorrecte\n");
        printf("Usage : entrez la touche 1, 2, 3 ou 4\n");
        choix_taille_module_rsa(choix);
    }
}

static void choix_loop_iteration(char*  choix, char** iter)
{
    int taille = 1;
    char* tmp = NULL;
    printf("\n\tChoisir le nombre iteration pour le timing attack :\n\t  ");
    *choix = getchar();
    if(*choix != '0' && *choix != '1' && *choix != '2' && *choix != '3' && *choix != '4' && *choix != '5' && *choix != '6' && *choix != '7' && *choix != '8' && *choix != '9')
    {
        while(getchar() != '\n');
        choix_loop_iteration(choix, iter);
        return;
    }
    *iter = malloc(sizeof(char) + 1);
    strncpy(*iter, choix, 1);
    while(1)
    {
        *choix = getchar();
        if(*choix == '\n')
            return;
        if(*choix != '0' && *choix != '1' && *choix != '2' && *choix != '3' && *choix != '4' && *choix != '5' && *choix != '6' && *choix != '7' && *choix != '8' && *choix != '9')
        {
            fprintf(stderr, "Erreur : saisie incorrecte\n");
            printf("Usage : entrez un nombre\n");
            if(tmp)
                free(tmp);
            free(*iter);
            choix_loop_iteration(choix, iter);
            return;
        }
        taille++;
        if(tmp)
            free(tmp);
        tmp = malloc(sizeof(char) * strlen(*iter) + 1);
        strncpy(tmp, *iter, strlen(*iter));
        free(*iter);
        *iter = malloc(sizeof(char) * taille + 1);
        strncpy(*iter, tmp, strlen(tmp));
        strncat(*iter, choix, 1);
    }
}

static void choix_padding(char* choix)
{
    int ctr = 1;
    printf("\n\tAvec padding PKCS#1 v1.5 ?\n\t- oui (1)\n\t- non (2)\n\t  ");
    *choix = getchar();
    while(getchar() != '\n')
        ctr++;

    if((*choix != '1' && *choix != '2') || ctr != 1)
    {
        fprintf(stderr, "Erreur : saisie incorrecte\n");
        printf("Usage : entrez la touche 1 ou 2\n");
        choix_padding(choix);
    }
}

void run()
{
    int i, nombre_iteration;
    char m = '\0', t = '\0', l = '\0', p = '\0';
    char* iteration = NULL;
    srand(getpid() + time(NULL)); //utiliser pour p, q, PKCS#1, (et le message clair)
    printf("\n\t\t--Timing Attack--\n");
    choix_mode_rsa(&m);
    choix_taille_module_rsa(&t);
    choix_loop_iteration(&l,&iteration);
    choix_padding(&p);

    if(t == '1')
    {
        printf("\n\t\tRSA-1024");
        prime_size = PRIME_LOW_SIZE;
    }

    if(t == '2')
    {
        printf("\n\t\tRSA-2048");
        prime_size = PRIME_MEDIUM_SIZE;
    }

    if(t == '3')
    {
        printf("\n\t\tRSA-3072");
        prime_size = PRIME_LONG_SIZE;
    }

    if(t == '4')
    {
        printf("\n\t\tRSA-4096");
        prime_size = PRIME_VERY_LONG_SIZE;
    }


    if(m == '1')
        printf("-CLASSIQUE");
    else
        printf("-MONTGOMERY");

    if(p == '1'){
        padding = true;
        printf("-AVEC_PADDING\n\n");
    }
    else{
        padding = false;
        printf("-SANS_PADDING\n\n");
    }

    nombre_iteration = atoi(iteration);

    for(i = 0; i < nombre_iteration; i++)
        run_rsa(m);
}

void run_rsa(const char mode)
{
    //--------------------CHRONO--------------------------
    clock_t t_cpu_deb = 0, t_cpu_fin = 0;
    struct timespec t_reel_deb = {0,0}, t_reel_fin = {0,0};
    double temps_reel_total = 0.0, temps_cpu_total = 0.0;
    debut_chrono(&t_cpu_deb,&t_reel_deb);
    //----------------------------------------------------
    
    mpz_t  m, e, p , q, n, c, phi_n, d, pkcs_sgn, s, hm, r, u, v, pgcd_bezout;
    mpz_init(m);        //clair
    mpz_init(e);        //exposant publique e : 1 < e < phi(n)
    mpz_init(p);        //nombre premier, premier facteur de n
    mpz_init(q);        //nombre premier, deuxième facteur de n
    mpz_init(n);        //produit des deux nombres premiers
    mpz_init(c);        //chiffré
    mpz_init(phi_n);    //phi(n) : (p - 1) * (q - 1)
    mpz_init(d);        //exposant privé : e^-1 mod ph(n)
    mpz_init(pkcs_sgn); //PKCS#1 signature : µ(m)
    mpz_init(s);        //signature : µ(m)^(d) mod n
    mpz_init(hm);       //hash du message pour PKCS#1 signature : H(m)
    mpz_init(r);
    mpz_init(u);
    mpz_init(v);
    mpz_init(pgcd_bezout);

    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) rand());
    gmp_printf("message d'origine : %Z0X\n", m);
    mpz_set(s, m);
    mpz_set(hm, m);
    hash(hm);
    //gmp_printf("\nhash : %Z0X\n", hm);
    mpz_set(pkcs_sgn, hm);

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

    if(mode == '2')
    {
        //générer R
        generer_R_montgomery(r);
        //bezout
        bezout(r, n, u, v, pgcd_bezout);
        //gmp_printf("R(%Zd) * v(%Zd) + N(%Zd) * u(%Zd)  \n\n", r, u, n, v);
    }

    //chiffrement de m
    if(mode == '1')
        chiffrement_RSA(m, e, n, c);
    else
        chiffrement_RSA_montgomery(m, e, n, c, v, n_size);
    gmp_printf("chiffré : %Z0X\n", c);

    //signature de m
    signature(pkcs_sgn, d, n, s);
    //gmp_printf("\nsignature : %Z0X\n", s);

    //vérification de s
    verification_signature(s, e, n, hm);

    //dechifrement de m
    if(mode == '1')
        dechiffrement_RSA(c, d, n, m);
    else
        dechiffrement_RSA_montgomery(c, d, n, m, v, n_size);
    gmp_printf("message déchiffré : %Z0X\n", m);

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_clear(phi_n);
    mpz_clear(d);
    mpz_clear(pkcs_sgn);
    mpz_clear(s);
    mpz_clear(hm);
    mpz_clear(r);
    mpz_clear(u);
    mpz_clear(v);
    mpz_clear(pgcd_bezout);

    //-----------------------------------FIN_CHRONO-----------------------------------------
    fin_chrono(&temps_cpu_total,t_cpu_deb,t_cpu_fin,&temps_reel_total,t_reel_deb,t_reel_fin);
    printf("\n");
}
