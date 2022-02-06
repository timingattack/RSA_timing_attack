#include <stdio.h>
#include <gmp.h>
#include "dechiffrement.h"
#include "square_multiply.h"

void dechiffrement_RSA(const mpz_t c, const mpz_t d, const mpz_t n, mpz_t m)
{
   square_and_multiply(c, d, n, m);
}