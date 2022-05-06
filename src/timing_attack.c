#include <stdio.h>
#include <stdlib.h>
#include "temps.h"
#include "timing_attack.h"

ENSEMBLE* initialiser_ensemble()
{
	ENSEMBLE* ens = malloc(sizeof(ENSEMBLE));
	
	if(!ens)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
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
		fprintf(stderr, "Erreur lors de l'allocation de l'élément.\n");
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

ELEMENT* retourner_premier_element(ENSEMBLE** ens)
{
	ELEMENT* premier = initialiser_element(-1.0);
	if((*ens)->elem == NULL)
		return premier;
	premier = (*ens)->elem;

	ELEMENT* elem = initialiser_element(0.0);
	elem->temps = premier->temps;
	
	(*ens)->elem = (*ens)->elem->suiv;
	premier->suiv = NULL;
	(*ens)->taille--;

	return elem;
}

void afficher_element(ELEMENT* elem, const char* nom)
{
	if(elem->temps == -1.0 || elem == NULL)
	{
		printf("\t\tL'element %s est vide.\n", nom);
		return;
	}
	printf("\t\tL'element %s est %f.\n", nom, elem->temps);
}

void test()
{
	A = initialiser_ensemble();
	B = initialiser_ensemble();
    ENSEMBLE* C = initialiser_ensemble();
    
    afficher_ensemble(A, "A");
    afficher_ensemble(B, "B");
    afficher_ensemble(C, "C");
    
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

    ELEMENT* ea = retourner_premier_element(&A);
    ELEMENT* eb = retourner_premier_element(&B);
    ELEMENT* ev = retourner_premier_element(&C);

    afficher_ensemble(A, "A");
    afficher_ensemble(B, "B");
    afficher_ensemble(C, "C");
    afficher_element(ev, "ev");
    
    free(e1);
    free(e2);
    free(e3);
    free(e4);
    free(e5);
    free(ea);
    free(eb);
    free(ev);
    free(A);
    free(B);
}
