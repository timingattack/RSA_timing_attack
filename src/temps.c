#include "temps.h"
#include <stdio.h> 
#include <sys/time.h>
#include <time.h>

/*
*	Fonction qui permet de calculer le temps réel.
*/
void temps_reel(struct timespec *temps_reel_t) {
	clock_gettime(CLOCK_MONOTONIC_RAW, temps_reel_t);
}

/*
*	Fonction qui permet de calculer le temps CPU.
*/
void temps_cpu(clock_t *temps_cpu_t) {
	*temps_cpu_t = clock();
}

/*
*	Fonction qui permet afficher le temps réel écoulé depuis le début de l'exécution.
*/
void afficher_temps_reel(double *temps_reel, struct timespec temps_reel_1, struct timespec temps_reel_2) {
	*temps_reel = (temps_reel_2.tv_sec - temps_reel_1.tv_sec);
	printf("temps réel = %.2f secondes\n",*temps_reel);
}

/*
*	Fonction qui permet afficher le temps CPU écoulé depuis le début de l'exécution.
*/
void afficher_temps_cpu(double *temps_cpu, clock_t temps_cpu_1, clock_t temps_cpu_2) {
	*temps_cpu = (temps_cpu_2 - temps_cpu_1) * 1e-6;
	printf("temps CPU = %.4f secondes\n",*temps_cpu);
}

/*
*	Fonction qui permet lancer le début du chrono.
*/
void debut_chrono(clock_t *tc, struct timespec *tr) {
	temps_cpu(tc);
	temps_reel(tr);
}

/*
*	Fonction qui permet lancer la fin du chrono.
*/
void fin_chrono(double *ttc, clock_t tdc, clock_t tfc, double *ttr, struct timespec tdr, struct timespec tfr) {
	temps_cpu(&tfc);
	temps_reel(&tfr);
	afficher_temps_cpu(ttc, tdc, tfc);
	afficher_temps_reel(ttr, tdr, tfr);
}

/*
*	Fonction qui permet lancer le début du chrono pour le timing attack.
*/
void debut_chrono_timing_attack(struct timespec *tc) {
	temps_reel(tc);
}

/*
*	Fonction qui permet lancer la fin du chrono timing attack.
*/
void fin_chrono_timing_attack(double *ttc, struct timespec tdc, struct timespec tfc) {
	temps_reel(&tfc);
	afficher_temps_reel(ttc, tdc, tfc);
}
