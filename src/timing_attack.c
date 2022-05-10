#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "temps.h"
#include "timing_attack.h"
#include "creation_des_cles.h" 		//pour n_size

//Initialisation des variables globales par défaut
ENSEMBLE_G* A = NULL;
ENSEMBLE_G* B = NULL;
unsigned int bit_cible = 0;
unsigned int bit_position = 0;
unsigned int TIMING_ATTACK_CONFIRMED = 0;

//Initialise les variables globales pour le timing attack
void initialiser_variables_globales_timing_attack()
{
    A = initialiser_ensemble_global();
    B = initialiser_ensemble_global();
}

ELEMENT* initialiser_element(const double temps)
{
	ELEMENT *elem = malloc(sizeof(ELEMENT));

	if(!elem)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'element.\n");
		exit(10);
	}

	elem->temps = temps;
	elem->suiv = NULL;
	
	return elem;
}

void afficher_element(const ELEMENT* elem, const char* nom)
{
	if(!elem)
	{
		printf("\t\tL'element %s est vide.\n\n", nom);
		return;
	}

	printf("\t\tL'element %s est %.9f\n\n", nom, elem->temps);
}

ENSEMBLE* initialiser_ensemble()
{
	ENSEMBLE* ens = malloc(sizeof(ENSEMBLE));
	
	if(!ens)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
		exit(11);
	}

	ens->elem = NULL;
	ens->fin = ens->elem;
	ens->taille = 0;

	return ens;
}

static bool verification_ensemble_non_null(const ENSEMBLE* ens)	//vérifie si un ensemble existe
{
	if(!ens)
		return 0;
	else
		return 1;
}

void afficher_ensemble(const ENSEMBLE* ens, const char* nom)
{	
	if(verification_ensemble_non_null(ens))
	{
		printf("\t\tEnsemble %s", nom);
		
		if(!ens->elem)
		{
			printf(" (taille 0) :\n\n");
			printf("\t\t   vide\n\n");
			return;
		}
		
		unsigned long int i = 1;
		printf(" (taille %lu) :\n\n", ens->taille);
		printf("\t\t   [%lu]: %f\n", i, ens->elem->temps);
		ELEMENT* e = ens->elem->suiv;
		while(e)
		{
			i++;
			printf("\t\t   [%lu]: %f\n", i, e->temps);
			e = e->suiv;
		}
		printf("\n");
		return;
	}
	fprintf(stderr,"\t\tL'ensemble %s n'existe pas.\n\n", nom);
}

void ajouter_element(ELEMENT* elem, ENSEMBLE** ens)
{
	if(verification_ensemble_non_null(*ens))
	{
		if((*ens)->taille == 0)
		{
			(*ens)->elem = elem;		//le premier élément
			(*ens)->fin = (*ens)->elem;	//le premier élément est aussi le dernier
			(*ens)->taille++;
		} else {
			(*ens)->fin->suiv = elem;	//l'élément suivant après l'avant dernier élément
			(*ens)->fin = elem;			//le dernier élément
			(*ens)->taille++;
		}
	}
}

ELEMENT* retourner_element(ENSEMBLE** ens)	//retourne le premier élément de la liste
{
	if(verification_ensemble_non_null(*ens))
	{
		if((*ens)->elem == NULL)
			return NULL;
		
		ELEMENT* premier = initialiser_element(0.0);
		premier = (*ens)->elem;

		ELEMENT* elem = initialiser_element(premier->temps);
		(*ens)->elem = (*ens)->elem->suiv;
		
		premier->suiv = NULL;
		(*ens)->taille--;
		free(premier);

		return elem;
	}
	return NULL;
}

/*
double temps_moyen(ENSEMBLE* ens)
{

}
*/

void supprimer_ensemble(ENSEMBLE** ens, const char* nom)
{
	if(verification_ensemble_non_null(*ens))
	{
		if(!(*ens)->elem)
		{
			*ens = NULL;
			free((*ens)->elem);
			free(*ens);
			printf("\t\tLa liste %s a ete supprimee.\n\n", nom);
			return;
		}

		ELEMENT* e = (*ens)->elem->suiv;
		while(e)
		{
			(*ens)->elem->suiv = NULL;
			(*ens)->elem = NULL;
			free((*ens)->elem);
			(*ens)->taille--;
			//printf("\t\tL'element %lu a ete supprime.\n", (*ens)->taille + 1);
			(*ens)->elem = e;
			e = e->suiv;
		}
		(*ens)->elem = NULL;
		free((*ens)->elem);
		(*ens)->taille--;
		//printf("\t\tL'element %lu a ete supprime.\n", (*ens)->taille + 1);
		*ens = NULL;
	}
	free(*ens);
	printf("\t\tLa liste %s a ete supprimee.\n\n", nom);
}

ENSEMBLE_G* initialiser_ensemble_global()
{
	unsigned int i = 0;

	ENSEMBLE_G* eg = malloc(sizeof(ENSEMBLE_G));	
	
	if(!eg)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble global.\n");
		exit(12);
	}

	eg->bit = malloc(sizeof(ENSEMBLE) * n_size);
	
	if(!eg->bit)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble global.\n");
		exit(13);
	}

	for(i = 0; i < n_size; i++)
	{
		eg->bit[i] = initialiser_ensemble();
	}

	return eg;
}

static bool verification_ensemble_global_non_null(ENSEMBLE_G* eg)	//vérifie si un ensemble global existe
{
	if(!eg)
	{
		return 0;
	}

	return 1;
}

void afficher_ensemble_global(ENSEMBLE_G* eg, const char* nom)
{
	unsigned int i = 0;
	if(verification_ensemble_global_non_null(eg))
	{
		for(i = 0; i < n_size; i++)
		{
			if(verification_ensemble_non_null(eg->bit[i]))
			{
				char str_bit[255];
				char* nom_liste = malloc(sizeof(char) * strlen(nom) + sizeof(char) * (int) (log10(i+1) + 1) + 1);
				nom_liste[0] = '\0';
				strncat(nom_liste, nom, strlen(nom));
				sprintf(str_bit, "%d", i+1);
				strncat(nom_liste, str_bit, strlen(str_bit));
				printf("%s\n", nom_liste);
				afficher_ensemble(eg->bit[i], nom_liste);
			}
		}
	}
}

void ajouter_element_global(ELEMENT* elem, ENSEMBLE_G** eg, const unsigned int i)
{
	if(verification_ensemble_global_non_null(*eg))
	{
		ajouter_element(elem, &((*eg)->bit[i]));
	}
}

ELEMENT* retourner_element_global(ENSEMBLE_G** eg, const unsigned int i)
{
	if(verification_ensemble_global_non_null(*eg))
	{
		ELEMENT* e = initialiser_element(0.0);
		e = retourner_element(&(*eg)->bit[i]);
		return e;
	}

	fprintf(stderr,"Erreur : L'ensemble global n'existe pas.\n");

	return NULL;
}

void supprimer_ensemble_global(ENSEMBLE_G** eg, const char* nom)
{
	if(verification_ensemble_global_non_null(*eg))
	{
		unsigned int i = 0;
		
		for(i = 0; i > n_size; i++)
		{
			supprimer_ensemble(&(*eg)->bit[i], nom);
		}
		*eg = NULL;
	}
	free(*eg);
	printf("\t\tLa liste globale %s a ete supprimee.\n\n", nom);
}

void test()
{
	n_size = 3;	//nombre d'ensemble (liste) dans A et B
	A = initialiser_ensemble_global();
	B = initialiser_ensemble_global();

	for(unsigned int i = 0; i < 3; i++){
		printf("[%d] : ", i);
		afficher_ensemble(A->bit[i], "A");
	}
	printf("\n");

	for(unsigned int i = 0; i < 3; i++){
		printf("[%d] : ", i);
		afficher_ensemble(B->bit[i], "B");
	}
	printf("\n");

    ENSEMBLE* C = initialiser_ensemble();
    
    printf("Avant ajout des elements.\n");
    afficher_ensemble(A->bit[0], "A");
    afficher_ensemble(B->bit[0], "B");
    afficher_ensemble(C, "C");
    
    ELEMENT* e1 = initialiser_element(12);
    ELEMENT* e2 = initialiser_element(79);
    ELEMENT* e3 = initialiser_element(3);
    ELEMENT* e4 = initialiser_element(56);
    ELEMENT* e5 = initialiser_element(93);
    
    ajouter_element_global(e1, &A, 0);
    ajouter_element_global(e2, &B, 0);
    ajouter_element_global(e3, &B, 0);
    ajouter_element_global(e4, &A, 0);
    ajouter_element_global(e5, &B, 0);

    printf("Apres ajout des elements.\n");
    afficher_ensemble(A->bit[0], "A");
    afficher_ensemble(B->bit[0], "B");
    afficher_ensemble(C, "C");

    ELEMENT* ea = retourner_element_global(&A, 0);
    ELEMENT* eb = retourner_element_global(&B, 0);
    ELEMENT* ec = retourner_element(&C);

    printf("Apres retour du premier element.\n");
    afficher_ensemble(A->bit[0], "A");
    afficher_ensemble(B->bit[0], "B");
    afficher_ensemble(C, "C");
    printf("Premier element retourne.\n");
    afficher_element(ea, "A");
    afficher_element(eb, "B");
    afficher_element(ec, "C");
    
    printf("Suppression des ensembles.\n");
    supprimer_ensemble(&C, "C");

    printf("Apres suppression des ensembles.\n");
    afficher_ensemble(C, "C");
    afficher_element(ea, "A");
    afficher_element(eb, "B");
    afficher_element(ec, "C");

    printf("Suppression des ensembles globaux.\n");
    supprimer_ensemble_global(&A, "A");
    supprimer_ensemble_global(&B, "B");
}
