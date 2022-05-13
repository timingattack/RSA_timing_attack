#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "temps.h"
#include "timing_attack.h"
#include "creation_des_cles.h" 		//pour n_size

//Initialisation des variables globales par défaut
ENSEMBLE* A = NULL;
ENSEMBLE* B = NULL;
unsigned int target_bit = 0;
unsigned int TIMING_ATTACK_CONFIRMED = 0;

//Initialise les variables globales pour le timing attack
void initialiser_variables_globales_timing_attack()
{
    A = initialiser_ensemble();
    B = initialiser_ensemble();
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

LISTE* initialiser_liste()
{
	LISTE* liste = malloc(sizeof(LISTE));
	
	if(!liste)
	{
		fprintf(stderr, "Erreur lors de l'allocation de la liste.\n");
		exit(11);
	}

	liste->elem = NULL;
	liste->fin = liste->elem;
	liste->taille = 0;
	liste->temps_moyen = 0.0;
	liste->temps_total = 0.0;

	return liste;
}

static bool verification_liste_non_null(const LISTE* liste)	//vérifie si une liste existe
{
	if(!liste)
		return 0;
	else
		return 1;
}

void afficher_liste_complete(const LISTE* liste, const char* nom)
{	
	if(verification_liste_non_null(liste))
	{
		printf("\t\t%s", nom);
		
		if(!liste->elem)
		{
			printf(" (taille 0) (temps moyen 0) (temps total 0) :\n\n");
			printf("\t\t   vide\n\n");
			return;
		}
		
		unsigned long int i = 1;
		printf(" (taille %lu) (temps moyen %f) (temps total %f) :\n\n", liste->taille, liste->temps_moyen, liste->temps_total);
		printf("\t\t   [%lu]: %f\n", i, liste->elem->temps);
		ELEMENT* e = liste->elem->suiv;
		while(e)
		{
			i++;
			printf("\t\t   [%lu]: %f\n", i, e->temps);
			e = e->suiv;
		}
		printf("\n");
		return;
	}
	fprintf(stderr,"\t\tLa liste %s n'existe pas.\n\n", nom);
}

void afficher_liste_simple(const LISTE* liste, const char* nom)
{	
	if(verification_liste_non_null(liste))
	{
		printf("%s", nom);
		
		if(!liste->elem)
		{
			printf("\n\ttaille : 0\n\ttemps moyen : 0\n\ttemps total : 0\n\n");
			return;
		}
		
		printf("\n\ttaille : %lu\n\ttemps moyen %f\n\ttemps total : %.6f\n\n", liste->taille, liste->temps_moyen, liste->temps_total);
		return;
	}
	fprintf(stderr,"\t\tLa liste %s n'existe pas.\n\n", nom);
}

void ajouter_element_liste(ELEMENT* elem, LISTE** liste)
{
	if(verification_liste_non_null(*liste))
	{
		if((*liste)->taille == 0)
		{
			(*liste)->elem = elem;			//le premier élément
			(*liste)->fin = (*liste)->elem;	//le premier élément est aussi le dernier
		} else {
			(*liste)->fin->suiv = elem;		//l'élément suivant après l'avant dernier élément
			(*liste)->fin = elem;			//le dernier élément
		}
		(*liste)->taille++;
		(*liste)->temps_total += elem->temps; 
	}
}

ELEMENT* retourner_element_liste(LISTE** liste)	//retourne le premier élément de la liste
{
	if(verification_liste_non_null(*liste))
	{
		if((*liste)->elem == NULL)
			return NULL;
		
		ELEMENT* premier = initialiser_element(0.0);
		premier = (*liste)->elem;

		ELEMENT* elem = initialiser_element(premier->temps);
		(*liste)->elem = (*liste)->elem->suiv;
		
		premier->suiv = NULL;
		(*liste)->taille--;
		free(premier);

		return elem;
	}
	return NULL;
}

void calculer_temps_moyen_liste(LISTE** liste)
{
	if(verification_liste_non_null(*liste))
	{
		(*liste)->temps_moyen = (*liste)->temps_total / (*liste)->taille;
	}
}

void supprimer_liste(LISTE** liste, const char* nom)
{
	if(verification_liste_non_null(*liste))
	{
		if(!(*liste)->elem)
		{
			*liste = NULL;
			free((*liste)->elem);
			free(*liste);
			printf("\t\tLa liste %s a ete supprimee.\n\n", nom);
			return;
		}

		ELEMENT* e = (*liste)->elem->suiv;
		while(e)
		{
			(*liste)->elem->suiv = NULL;
			(*liste)->elem = NULL;
			free((*liste)->elem);
			(*liste)->taille--;
			//printf("\t\tL'element %lu a ete supprime.\n", (*liste)->taille + 1);
			(*liste)->elem = e;
			e = e->suiv;
		}
		(*liste)->elem = NULL;
		free((*liste)->elem);
		(*liste)->taille--;
		//printf("\t\tL'element %lu a ete supprime.\n", (*liste)->taille + 1);
		*liste = NULL;
	}
	free(*liste);
	printf("\t\tLa liste %s a ete supprimee.\n\n", nom);
}

ENSEMBLE* initialiser_ensemble()
{
	unsigned int i = 0;

	ENSEMBLE* ens = malloc(sizeof(ENSEMBLE));	
	
	if(!ens)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
		exit(12);
	}

	ens->bit = malloc(sizeof(LISTE) * n_size);
	
	if(!ens->bit)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
		exit(13);
	}

	for(i = 0; i < n_size; i++)
	{
		ens->bit[i] = initialiser_liste();
	}

	return ens;
}

static bool verification_ensemble_non_null(ENSEMBLE* ens)	//vérifie si un ensemble global existe
{
	if(!ens)
	{
		return 0;
	}

	return 1;
}

void afficher_ensemble_complet(ENSEMBLE* ens, const char* nom)
{
	unsigned int i = 0;
	if(verification_ensemble_non_null(ens))
	{
		for(i = 0; i < n_size; i++)
		{
			if(verification_liste_non_null(ens->bit[i]))
			{
				char str_bit[255];
				char nom_liste[9] = "bit "; 
				char* nom_ensemble = malloc(sizeof(char) * strlen(nom) + sizeof(char) * 2 + sizeof(char) * (int) (log10(i+1) + 1) + 1);

				nom_ensemble[0] = '\0';
				sprintf(str_bit, "%d", i);
				
				strncat(nom_ensemble, nom, strlen(nom));
				strncat(nom_ensemble, "[", 1);
				strncat(nom_ensemble, str_bit, strlen(str_bit));
				strncat(nom_ensemble, "]", 1);
				printf("%s\n", nom_ensemble);
				
				sprintf(str_bit, "%d", i+1);
				strncat(nom_liste, str_bit, strlen(str_bit));
				afficher_liste_complete(ens->bit[i], nom_liste);
			}
		}
	}
}

void afficher_ensemble_simple(ENSEMBLE* ens, const char* nom)
{
	unsigned int i = 0;
	if(verification_ensemble_non_null(ens))
	{
		for(i = 0; i < n_size; i++)
		{
			if(verification_liste_non_null(ens->bit[i]))
			{
				char str_bit[255];
				char nom_liste[9] = "bit "; 
				char* nom_ensemble = malloc(sizeof(char) * strlen(nom) + sizeof(char) * 5 + sizeof(char) * 9 + sizeof(char) * (int) (log10(i+1) + 1) + 1);

				nom_ensemble[0] = '\0';
				sprintf(str_bit, "%d", i);
				
				strncat(nom_ensemble, nom, strlen(nom));
				strncat(nom_ensemble, "[", 1);
				strncat(nom_ensemble, str_bit, strlen(str_bit));
				strncat(nom_ensemble, "]", 1);
				strncat(nom_ensemble, " : ", 3);

				sprintf(str_bit, "%d", i+1);
				strncat(nom_liste, str_bit, strlen(str_bit));

				strncat(nom_ensemble, nom_liste, strlen(nom_liste));
				printf("%s\n", nom_ensemble);

				afficher_liste_simple(ens->bit[i], "");
			}
		}
	}
}

void ajouter_element(ELEMENT* elem, ENSEMBLE** ens, const unsigned int i)
{
	if(verification_ensemble_non_null(*ens))
	{
		ajouter_element_liste(elem, &((*ens)->bit[i]));
	}
}

ELEMENT* retourner_element(ENSEMBLE** ens, const unsigned int i)
{
	if(verification_ensemble_non_null(*ens))
	{
		ELEMENT* e = initialiser_element(0.0);
		e = retourner_element_liste(&(*ens)->bit[i]);
		return e;
	}

	fprintf(stderr,"Erreur : L'ensemble n'existe pas.\n");

	return NULL;
}

void calculer_temps_moyen(ENSEMBLE** ens)
{
	unsigned int i;
	if(verification_ensemble_non_null(*ens))
	{
		for(i = 0; i < n_size; i++)
		{
			calculer_temps_moyen_liste(&(*ens)->bit[i]);
		}
	}
}

void supprimer_ensemble(ENSEMBLE** ens, const char* nom)
{
	if(verification_ensemble_non_null(*ens))
	{
		unsigned int i = 0;
		
		for(i = 0; i > n_size; i++)
		{
			supprimer_liste(&(*ens)->bit[i], nom);
		}
		*ens = NULL;
	}
	free(*ens);
	printf("\t\tL'ensemble %s a ete supprimee.\n\n", nom);
}

void test()
{
	n_size = 3;	//nombre d'ensemble (liste) dans A et B
	A = initialiser_ensemble();
	B = initialiser_ensemble();

	
	afficher_ensemble_complet(A, "A");
	afficher_ensemble_complet(B, "A");

    LISTE* C = initialiser_liste();
    
    printf("Avant ajout des elements.\n");
    afficher_liste_complete(A->bit[0], "A");
    afficher_liste_complete(B->bit[0], "B");
    afficher_liste_complete(C, "C");
    
    ELEMENT* e1 = initialiser_element(12);
    ELEMENT* e2 = initialiser_element(79);
    ELEMENT* e3 = initialiser_element(3);
    ELEMENT* e4 = initialiser_element(56);
    ELEMENT* e5 = initialiser_element(93);
    
    ajouter_element(e1, &A, 0);
    ajouter_element(e2, &B, 0);
    ajouter_element(e3, &B, 0);
    ajouter_element(e4, &A, 0);
    ajouter_element(e5, &B, 0);

    printf("Apres ajout des elements.\n");
    afficher_liste_complete(A->bit[0], "A");
    afficher_liste_complete(B->bit[0], "B");
    afficher_liste_complete(C, "C");

    ELEMENT* ea = retourner_element(&A, 0);
    ELEMENT* eb = retourner_element(&B, 0);
    ELEMENT* ec = retourner_element_liste(&C);

    printf("Apres retour du premier element.\n");
    afficher_liste_complete(A->bit[0], "A");
    afficher_liste_complete(B->bit[0], "B");
    afficher_liste_complete(C, "C");
    printf("Premier element retourne.\n");
    afficher_element(ea, "A");
    afficher_element(eb, "B");
    afficher_element(ec, "C");
    
    printf("Suppression des listes.\n");
    supprimer_liste(&C, "C");

    printf("Apres suppression des listes.\n");
    afficher_liste_complete(C, "C");
    afficher_element(ea, "A");
    afficher_element(eb, "B");
    afficher_element(ec, "C");

    printf("Suppression des ensembles.\n");
    supprimer_ensemble(&A, "A");
    supprimer_ensemble(&B, "B");
}
