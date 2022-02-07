#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "miller_rabin.h"
#include "square_multiply.h"

//quantité de nombres premiers utilisés comme base dans l'algorithme de Miller-Rabin
#define BASES_NUMBERS 168
//les nombres premiers
int bases_miller_rabin[BASES_NUMBERS] = {2,
		3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89
		,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173
		,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263
        ,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359
        ,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457
        ,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569
        ,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659
        ,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769
        ,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881
        ,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997
    };

//vérifie si n est composé (si a est un temoin de miller)
static bool est_compose(const mpz_t a, const mpz_t d, const mpz_t n, const mpz_t z, const mpz_t s)
{
	mpz_t x, r;
	mpz_init(x);
	mpz_init(r);
	mpz_set(r, s);

	//calcul x = a^d mod n
	square_and_multiply(a, d, n, x);
	
	//si x = 1 ou x = n - 1
	if(!(mpz_cmp_ui(x, 1)) || !(mpz_cmp(x, z)))
	{
		mpz_clear(x);
		mpz_clear(r);
		
		return false;	//n est SPRP (fortement premier)
	}

	//s - 1 itérations
	while(mpz_cmp_ui(r, 0))	
	{
		//x = x^2 mod n
		mpz_mul(x, x, x); mpz_mod(x, x, n);
		
		//si x = n - 1
		if(!(mpz_cmp(x, z)))
		{
			mpz_clear(x);
			mpz_clear(r);

			return false;	//n est SPRP (fortement premier)
		}

		mpz_sub_ui(r, r, 1);
	}

	mpz_clear(x);
	mpz_clear(r);

	return true; //n est composé (pas premier);
}

/* Test si a est un témoin de Miller pour n 
* - condition n ≥ 3 et n est impair
* - formule : n - 1 = 2^s * d  , avec s > 0
*/
static bool temoin_de_miller(const mpz_t n, const mpz_t a)
{
	bool booleen;
	mpz_t z, s, d, andr, msk;
	mpz_init(z);		//n - 1
	mpz_init(s);		//exposant s
	mpz_init(d);		//facteur d
	mpz_init(andr);		//résultat d'un AND
	mpz_init(msk);		//masque de ...0001 en binaire
	mpz_set_ui(msk, 1);

	//z = n - 1
 	mpz_sub_ui(z, n, 1);

 	/* Calcul de s et d pour 2^s * d = n - 1 : 
	* - calculer s et d revient à enlever tous les bits de poids faibles qui sont égales à 0
	* et à ne garder que les bits de poids forts (on s'arrête au premier bit de poids faible qui vaut 1)
	* - s = le nombre de bits de poids faibles que l'on a retiré
	* - d = la partie des bits de poids forts
 	*/
 	mpz_set(d, z);
 	
 	//si le bits de poids le plus faible est égale à 0
 	while(!(mpz_cmp_ui(andr, 0)))
 	{
 		mpz_tdiv_q_2exp(d, d, 1);	//décallage à droite (tant que les bits de poids faibles de n - 1 sont égale à 0)
 		mpz_add_ui(s, s, 1);		//on incrémente s de +1 à chaque fois que l'on effectue un décallage à droite
 		mpz_and(andr, d, msk);		//on détermine la valuer du bit de poids le plus faible (andr = 1 si le bit égale à 1, 0 sinon)
 	}
	
	//gmp_printf("z = %Zd, s = %Zd, d = %Zd\n", z, s, d);	//décommenter pour afficher z, s et d

 	//vrai si n est composé,
 	//faux si n est SPRP (fortement premier)
 	booleen = est_compose(a, d, n, z, s);

 	mpz_clear(z);
    mpz_clear(s);
    mpz_clear(d);

 	return booleen;
}

//test si n est premier
bool miller_rabin(const mpz_t n)
{
	int i, j;
	mpz_t a;
	mpz_init(a);

	//si n < 3
	if((mpz_cmp_ui(n, 3) < 0))
	{
		mpz_clear(a);

		return (mpz_cmp_ui(n, 2) == 0);	//retourne vrai si n = 2
	}
	
	for(i = 0; i < BASES_NUMBERS; i++)
	{
		mpz_set_ui(a, bases_miller_rabin[i]);

		if(mpz_cmp(n, a) == 0)
		{
			mpz_clear(a);

			return true;
		}
		
		//test si n est composé
		if(temoin_de_miller(n, a))
		{
			mpz_clear(a);

			return false;	//n n'est pas premier
		}

		mpz_init(a);
	}
	
	mpz_clear(a);

	return true;	//n est premier
}

//génère un nombre premier sur b bits
void generer_un_nombre_premier(mpz_t p, int b)
{
	int i = 0, j;
	double b_value, limit_value;
	char* random_number = malloc(sizeof(char) * b + 1); //chaine de b caractères
	
	if(!random_number)
    {
        fprintf(stderr, "Erreur: échec du malloc pour la génération du nombre premier.\n");
        exit(2);
    }

	//si le nombre est sur 2 bits on sort
	if(b < 2)
	{
		free(random_number);

		fprintf(stderr, "Erreur: le nombre de bit pour générer le nombre premier est insuffisant.\n");

		exit(4);
	}
	
	while(true)
	{
		i++;
		//réinitialise la chaine de bits
		strncpy(random_number,"\0", strlen(random_number));

		//génère un nombre aléatoire entre 0 et 2^b - 1
		strncat(random_number, "1", 1);	//met le bit de poids le plus fort à 1
		for(j = 1; j < b - 1; j++)		//remplie aléatoirement la chaine de 0 ou 1
		{
			//nombre aléatoire entre 0 et 1
			b_value = (double) random() / RAND_MAX;
			//limite d'acceptation aléatoire pour les bits à 1
			limit_value = (double) random() / RAND_MAX;
			
			//si nombre < limite alors b = 0
			//sinon si ≥ limite alors b = 1
			if(b_value < limit_value)
			{
				strncat(random_number, "0", 1);
			}
			else if(b_value >= limit_value)
			{
				strncat(random_number, "1", 1);
			}

			//printf("bit %s avec proba de %f\n", random_number, limit_value); //décommenter pour afficher la probabilité pour que le bit soit égale à 0 ou 1
		}
		strncat(random_number, "1", 1);	//met le bit de poids le plus faible à 1
		
		//printf("resultat du binaire = %s\n", random_number);	//décommenter pour afficher le résultat en binaire

		//convertie le nombre aléatoire représenté en binaire en nombre décimal
		mpz_set_str(p, random_number, 2);
		//gmp_printf("resultat décimal = %Zd\n", p);	//décommenter pour afficher le résultat en décimal

		//vérifie si le nombre aléatoire est premier
		if(miller_rabin(p))
		{
			free(random_number);

			//gmp_printf("le nombre %Zd est premier (trouvé après %d essais)\n\n", p, i);	//décommenter pour afficher le résultat
	
			return;
		}

		//printf("\n");	//à décommenter lors de l'affichage

		mpz_init(p);

		//décommenter pour avoir une limite de tentative de génération du nombre premier
		/*
		if(i >= b * 100)
		{
			break;
		}
		*/
	}
	//arrive ici uniquement que la limite de tentative de génération (si activée) à été atteinte

	free(random_number);

	fprintf(stderr,"la génération du nombre premier à échoué après %d essais\n\n", i);
	
	exit(3);	//le nombre maximum de tentative de génération à été atteint, on arrête le programme
}
