#ifndef _TIMING_ATTACK_H_
#define _TIMING_ATTACK_H_

typedef struct element {
	double temps;
	struct element* suiv;
} ELEMENT; 						//représente un élément de l'ensemble A ou B.

typedef struct ensemble {
	ELEMENT* elem;
	ELEMENT* fin;
	unsigned long int taille;
} ENSEMBLE; 					//représente l'ensemble A ou B sous la forme d'une simple liste chaînée.

ENSEMBLE* initialiser_ensemble();
ELEMENT* initialiser_element(double temps);
void ajouter_element(ELEMENT *elem, ENSEMBLE **ens);
void afficher_ensemble(ENSEMBLE* ens, const char* nom);
ELEMENT* retourner_dernier_element(ENSEMBLE* ens);

void test(); //fonction de test pour tester les fonctions

#endif /* _TIMING_ATTACK_H_ */
