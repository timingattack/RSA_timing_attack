#include <stdio.h>
#include <stdlib.h>
#include "temps.h"
#include "timing_attack.h"

ENSEMBLE* initialiser_ensemble()
{
	ENSEMBLE* ens = malloc(sizeof(ENSEMBLE));
	
	if(!ens)
	{
		fscanf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
		exit(10);
	}

	ens->elem = NULL;
	ens->fin = ens->elem;
	ens->taille = 0;

	return ens;
}

ELEMENT* initialiser_element(double temps)
{
	ELEMENT *elem = malloc(sizeof(ELEMENT));
	
	if(!elem)
	{
		fscanf(stderr, "Erreur lors de l'allocation de l'élément.\n");
		exit(11);
	}

	elem->temps = temps;
	elem->suiv = NULL;
	
	return elem;
}

void ajouter_element(ELEMENT *elem, ENSEMBLE **ens)
{
	if((*ens)->taille == 0)
	{
		(*ens)->elem = elem;		//le premier élément
		(*ens)->fin = (*ens)->elem;	//le premier élément est aussi le dernier
		(*ens)->taille++;
	}
	else
	{
		(*ens)->fin->suiv = elem;	//l'élément suivant après l'avant dernier élément
		(*ens)->fin = elem;			//le dernier élément
		(*ens)->taille++;
	}
}

void afficher_ensemble(ENSEMBLE* ens, const char* nom)
{
	unsigned long int i = 1;
	printf("\n\t\tEnsemble %s (taille %lu) :\n", nom, ens->taille);
	if(!ens->elem) {
		printf("\t\t   vide\n\n");
		return;
	}
	printf("\t\t   [%lu]: %f\n", i, ens->elem->temps);
	ELEMENT* e = ens->elem->suiv;
	while(e)
	{
		i++;
		printf("\t\t   [%lu]: %f\n", i, e->temps);
		e = e->suiv;
	}
	printf("\n");
}

void test()
{
	ENSEMBLE* A = initialiser_ensemble();
    ENSEMBLE* B = initialiser_ensemble();
    
    afficher_ensemble(A, "A");
    afficher_ensemble(B, "B");
    
    ELEMENT* e1 = initialiser_element(12);
    ELEMENT* e2 = initialiser_element(79);
    ELEMENT* e3 = initialiser_element(3);
    ELEMENT* e4 = initialiser_element(56);
    ELEMENT* e5 = initialiser_element(93);
    
    ajouter_element(e1, &A);
    ajouter_element(e2, &B);
    ajouter_element(e3, &B);
    ajouter_element(e4, &A);
    ajouter_element(e5, &B);
    
    afficher_ensemble(A, "A");
    afficher_ensemble(B, "B");
    
    free(e1);
    free(e2);
    free(e3);
    free(e4);
    free(e5);
    free(A);
    free(B);
}
