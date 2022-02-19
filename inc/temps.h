#ifndef _TEMPS_H_
#define _TEMPS_H_
#include <sys/time.h>
#include <time.h>

void temps_reel(struct timespec *t);
void temps_cpu(clock_t *t);
void afficher_temps_reel(double *tt, struct timespec td, struct timespec tf);
void afficher_temps_cpu(double *tt, clock_t td, clock_t tf);
void debut_chrono(clock_t *tc, struct timespec *tr);
void fin_chrono(double *ttc, clock_t tdc, clock_t tfc, double *ttr, struct timespec tdr, struct timespec tfr);

#endif /* _TEMPS_H_ */
