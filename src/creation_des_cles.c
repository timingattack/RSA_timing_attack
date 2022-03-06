#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "creation_des_cles.h"
#include "square_multiply.h"
#include "miller_rabin.h"

//nombre de bits qui seront utilisés pour p et q
#define PRIME_NUMBER_SIZE prime_size

void generer_npq(mpz_t n, mpz_t p, mpz_t q)
{
    mpz_init(n);
    mpz_init(p);
    mpz_init(q);

    generer_un_nombre_premier(p, PRIME_NUMBER_SIZE); 
    generer_un_nombre_premier(q, PRIME_NUMBER_SIZE);

    if(mpz_cmp(p, q))
    {
        mpz_mul(n, p, q);
        return;
    } else {
        generer_npq(n, p, q);
        return;
    }
}

void generer_exposant_public(mpz_t e)
{
    mpz_set_ui(e, 65537);
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
        fprintf(stderr,"Erreur: l'inverse modulaire de l'exposant public n'existe pas.\n");
        mpz_clear(pgcd_r);
        exit(5);
    } else {
        mpz_gcd(pgcd_r, e, d);
        //si le PGCD(e,d) ≠ 1 on arrête le programme
        if(mpz_cmp_ui(pgcd_r, 1))
        {
            fprintf(stderr,"Erreur: l'inverse modulaire de l'exposant public n'est pas premier avec l'exposant privé d.\n");
            mpz_clear(pgcd_r);
            exit(6);
        }
    }
}
