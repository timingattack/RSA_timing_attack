#include "rsa.h"
#include <unistd.h>
#include <time.h>


int main (){
    mpz_t m;
    mpz_init(m);
    //gen_key(512);
    srand(getpid()+time(NULL));
    unsigned long int grain = rand();
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state,grain);
    gen_random(m, state, 512);
    printf("message aléatoire = "); mpz_out_str(stdout, 16, m); printf("\n");
    chiffrement(m,1024);
    printf("chiffrement       = "); mpz_out_str(stdout, 16, m); printf("\n");
    dechiffrement(m,1024);
    printf("dechiffrement     = "); mpz_out_str(stdout, 16, m); printf("\n");
    signature(m,1024);
    printf("signature         = "); mpz_out_str(stdout, 16, m); printf("\n");
    mpz_clear(m);
    return 0;
}