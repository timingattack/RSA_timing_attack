#ifndef _TIMING_ATTACK_H_
#define _TIMING_ATTACK_H_

typedef struct element {
	double temps;
} ELEMENT; // représente un élément de l'ensemble A ou B.

typedef struct ensemble {
	ELEMENT elem;
	ELEMENT* suivant;
	ELEMENT* fin;
	unsigned long int taille;
} ENSEMBLE; // représente l'ensemble A ou B sous la forme d'une simple liste chaînée.

#endif /* _TIMING_ATTACK_H_ */
