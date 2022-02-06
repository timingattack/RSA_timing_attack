#include <stdio.h>
#include <gmp.h>
#include "square_multiply.h"

#define color(param) printf("\033[%sm", param)  //aide à l'affichage
#define KEY_SIZE (unsigned int) mpz_sizeinbase(exp, 2) //nombre de bits de e (clé publique) ou de d (clé privée)

/* Exponentiation rapide (pour chiffrer et déchiffrer !)
* x = m (message) / c (chiffré)
* exp = e (clé publique) / d (clé privée)
* n = p * q
* y = c (chiffré) / m (message)
*/
void square_and_multiply(const mpz_t x, const mpz_t exp, const mpz_t n, mpz_t y)
{
    unsigned int i;
    mpz_t msk, rshiftr, andr;
    
    mpz_init(msk);      //masque de ...0001 en binaire
    mpz_init(rshiftr);  //résultat d'un décallage à droite
    mpz_init(andr);     //résultat d'un AND
    mpz_set_ui(msk,1);  
    mpz_set_ui(y,1);

    //décommenter lors de l'affichage en couleur
    /*
    gmp_printf("\tparam : x = %Zd, exposant = ", x);
    color("1");
    color("32");
    gmp_printf("%Zd", exp);
    color("0");
    gmp_printf(", n = %Zd\n\n", n);
    */

    for (i = KEY_SIZE; i > 0; i--)
    {
        //square
        mpz_mul(y, y, y); mpz_mod(y, y, n);
        
        //décommenter lors de l'affichage en couleur
        /*
        color("35");
        gmp_printf("square : ");
        color("0");
        gmp_printf("y = %Zd\n", y);
        */

        //vérification bit de l'exposant exp
        mpz_tdiv_q_2exp(rshiftr, exp, i - 1);
        mpz_and(andr, rshiftr, msk);
        if(!(mpz_cmp_ui(andr, 1)))
        {   
            //décommenter lors de l'affichage en couleur
            /*
            gmp_printf("\nbit exp%d = ", i - 1);
            color("1");
            color("32");
            gmp_printf("1\n\n");
            color("0");
            */

            //multiply
            mpz_mul(y, y, x); mpz_mod(y, y, n);
            
            //décommenter lors de l'affichage en couleur
            /*
            color("36");
            gmp_printf("multiply : ");
            color("0");
            gmp_printf("y = %Zd\n", y);
            */

        } else {
            //décommenter lors de l'affichage en couleur
            /*
            gmp_printf("\nbit exp%d = ", i - 1);
            color("1");
            color("32");
            gmp_printf("0\n");
            color("0");
            */
        }
        //gmp_printf("--------------------\n"); //décommenter lors de l'affichage en couleur
    }

    //gmp_printf("\t y = %Zd mod %Zd\n\n", y, n);   //décommenter pour afficher y = x^exp mod n

    mpz_clear(msk);
    mpz_clear(rshiftr);
    mpz_clear(andr);
}

