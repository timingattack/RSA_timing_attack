#include "rsa.h"

/**
 * @brief           fonction pour generer un random. Elle est appelée dans gen_key
 * 
 * @param tmp       la variable qui contiendra le random
 * @param state     parametre pour initialiser la fonction urandomb
 * @param bits      le nombre de bits du rand exemple 512, 1024, ...
 */
void gen_random(mpz_t tmp, gmp_randstate_t state, mp_bitcnt_t bits){
    mpz_init2(tmp,bits);
    mpz_urandomb(tmp,state,bits);
}

/**
 * @brief           fonction permetant la generation des clés pulic et secrete. elles sont stockés dans deux fichiers distingue
 * 
 * @param bits      le nombre de bits RSA exemple 1024, 2048, ...
 */
void gen_key(mp_bitcnt_t bits){
    gmp_randstate_t state;
    srand(getpid()+time(NULL));
    unsigned long int grain = rand();
    unsigned long int grain2;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state,grain);
    mpz_t val;
    char pk[50];
    char sk[50];
    sprintf(pk,"id_rsa%ld.pub",bits);
    sprintf(sk,"id_rsa%ld",bits);
    bits /= 2;
    gen_random(val, state, bits);
    mpz_t p;
    mpz_t q;
    mpz_nextprime(p,val);               // cette fonction met le prochain nombre premier apres val dans p
    // on calcule q telque p different de q
    do
    {
        grain2 = rand();
        gmp_randseed_ui(state,grain2);
        gen_random(val, state, bits);
        mpz_nextprime(q,val);
    } while (mpz_cmp(p,q)==0);
    mpz_t n;
    mpz_mul(n, p, q);
    FILE *f1 = fopen(pk,"w+");
    mpz_t e;
    mpz_init_set_ui(e, 65537);  // valeur arbitraire souvant utiliser pour le chiffrement RSA, e est un nombre premier. vu en TD
    gmp_fprintf(f1,"%Zx\n",e);  // ecriture de e dans le fichier en base 16
    gmp_fprintf(f1,"%Zx\n",n);  // ecriture de n dans le fichier en base 16
    fclose(f1);
    mpz_t phi,d;
    FILE *f2 = fopen(sk,"w+");
    gmp_fprintf(f2,"%Zx\n",p);
    gmp_fprintf(f2,"%Zx\n",q);
    calcul_phi(phi, p, q);
    mpz_invert(d, e, phi);
    gmp_fprintf(f2,"%Zx\n",phi);
    gmp_fprintf(f2,"%Zx\n",d);
    fclose(f2);
    mpz_clear(phi);
    mpz_clear(n);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(val);
}

/**
 * @brief       fonction pour calculer phi 
 * 
 * @param phi   le resultat. secrete
 * @param p     p secrete
 * @param q     q secrete
 */
void calcul_phi(mpz_t phi, mpz_t p, mpz_t q){
    mpz_t p1, q1;
    mpz_init_set(p1,p);
    mpz_init_set(q1,q);
    mpz_sub_ui(p1,p1,1);
    mpz_sub_ui(q1,q1,1);
    mpz_mul(phi, p1, q1);
    mpz_clear(p1);
    mpz_clear(q1);
}

/**
 * @brief       calcul RSA selon la methode square and multiply vu en cours
 * 
 * @param m     le message, clair ou chiffré
 * @param e     l'exposant, e ou d selon l'utilisation
 * @param n     le module
 */
void square_and_multiply(mpz_t m, mpz_t e, mpz_t n){
    FILE *f1 = fopen("bin_key","w+");
    mpz_out_str(f1, 2, e);   // on ecrit en binaire la clé dans un fichier nommé bin_key
    rewind(f1);
    mpz_t c;                // le message chiffré c = m exp(e) mod n
    mpz_init_set_ui(c,1);   // on initialise c = 1
    int indice = fgetc(f1);
    while (indice != EOF)
    {
        mpz_t multi;
        // c = cxc mod n Square
        mpz_mul(multi,c,c);
        mpz_init_set(c,multi);
        mpz_mod(c,multi,n);
        if (indice == '1')
        {
            // c = cxm mod n 
            mpz_mul(multi,c,m);
            mpz_init_set(c,multi);
            mpz_mod(c,multi,n);
        }
        indice = fgetc(f1);
    }
    mpz_init_set(m,c);
    mpz_clear(c);
    fclose(f1);
    remove("bin_key");   // suppression du fichier
}

/**
 * @brief on chiffre le message passé en parametre
 * 
 * @param m le massage m qui devient le message chiffré à la fin de fonction
 * @param bits  le nombre de bits pour le chiffrement RSA expelme 1024, 2048...
 */
void chiffrement(mpz_t m, mp_bitcnt_t bits){
    char pk[50];
    sprintf(pk,"id_rsa%ld.pub",bits);
    FILE *f = NULL;
    if (access(pk,F_OK)==-1)
    {
       gen_key(bits);
    }
    f = fopen(pk,"r");
    if(f != NULL){
        mpz_t e,n;
        mpz_init(e);
        mpz_init(n);
        gmp_fscanf(f,"%Zx\n",e);        // on recupere e
        gmp_fscanf(f,"%Zx\n",n);        // on recupere n
        fclose(f);
        square_and_multiply(m,e,n);     // appel de la fonction pour le calcul RSA
        mpz_clear(e);
        mpz_clear(n);
    }
    else{
        printf("Impossible d'ouvrir le fichier contenant la clé de chiffrement");
    }
    
}
/**
 * @brief           fonction pesqu'identique à la fonction chiffrement. on ne genere pas de clé s'il n'existe pas, la fonction ne fera rien dans ce cas
 * 
 * @param m         le messase à dechiffré
 * @param bits      le nombre de bits du RSA
 */
void dechiffrement(mpz_t m, mp_bitcnt_t bits){
    char sk[50];
    sprintf(sk,"id_rsa%ld",bits);
    FILE *f = NULL;
    f = fopen(sk,"r");
    if(f != NULL){
        mpz_t p, q,phi, d, n;
        mpz_init(d);
        mpz_init(n);
        mpz_init(p);
        mpz_init(q);
        mpz_init(phi);
        gmp_fscanf(f,"%Zx\n",p);
        gmp_fscanf(f,"%Zx\n",q);
        gmp_fscanf(f,"%Zx\n",phi);
        gmp_fscanf(f,"%Zx\n",d);
        mpz_mul(n,p,q);
        //printf("e   = "); mpz_out_str(stdout, 2, e); printf("\n");
        //printf("n   = "); mpz_out_str(stdout, 2, n); printf("\n");
        fclose(f);
        square_and_multiply(m,d,n);
        mpz_clear(d);
        mpz_clear(n);
        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(phi);
    }
    else{
        printf("Impossible d'ouvrir le fichier contenant la clé de dechiffrement");
    }
}
/**
 * @brief           // faction identique au de chiffrement. S'il n'y a pas de clé, elle est generé
 * 
 * @param m         // le message à signé
 * @param bits      // le nombre de bits RSA, ex: 1024, 2048, ...
 */
void signature(mpz_t m, mp_bitcnt_t bits){
    char sk[50];
    sprintf(sk,"id_rsa%ld",bits);
    if (access(sk,F_OK)==-1)
    {
       gen_key(bits);
    }
    dechiffrement(m, bits);
}