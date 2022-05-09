#ifndef _TIMING_ATTACK_H_
#define _TIMING_ATTACK_H_

#include "creation_des_cles.h" 

typedef struct element {
	double temps;
	struct element* suiv;
} ELEMENT; 				//représente un élément de l'ensemble A ou B.

typedef struct ensemble {
	ELEMENT* elem;
	ELEMENT* fin;
	unsigned long int taille;
} ENSEMBLE; 			//représente l'ensemble A ou B (pour un bit) sous la forme d'une simple liste chaînée.

typedef struct ensemble_globale {
	ENSEMBLE** bit;
} ENSEMBLE_G;			//représente l'ensemble A et B (pour tous les bits) sous la forme d'un tableau de listes simplements chaînées.
						//tableau de taille n, liste de taille >= nombre d'itération
						//l'indice du tableau représente le bit, la liste représente les temps mesurés

//Fonctions pour les éléments
ELEMENT* initialiser_element(const double temps);
void afficher_element(const ELEMENT* elem, const char* nom);

//Fonctions pour les ensembles
ENSEMBLE* initialiser_ensemble();
void ajouter_element(ELEMENT* elem, ENSEMBLE** ens);
ELEMENT* retourner_element(ENSEMBLE** ens);
void afficher_ensemble(const ENSEMBLE* ens, const char* nom);
void supprimer_ensemble(ENSEMBLE** ens, const char* nom);

//Fonctions pour les ensembles globaux
ENSEMBLE_G* initialiser_ensemble_global();
void ajouter_element_global(ELEMENT *elem, ENSEMBLE_G** eg, const unsigned int bit);
ELEMENT* retourner_element_global(ENSEMBLE_G** eg, const unsigned int bit);
void supprimer_ensemble_global(ENSEMBLE_G** eg, const char* nom);

//Variables globales
ENSEMBLE_G* A;
ENSEMBLE_G* B;
unsigned int bit_position;	//le bit courant lors du timing attack
unsigned int bit_cible;		//le bit ciblé lors du timing attack

void test(); 			//fonction de test pour tester les fonctions (à faire: à retirer lorsque timing attack fini)

#endif /* _TIMING_ATTACK_H_ */
