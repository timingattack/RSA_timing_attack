#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "chiffrement.h"
#include "square_multiply.h"

void chiffrement_RSA(const mpz_t m, const mpz_t e, const mpz_t n, mpz_t c)
{
   square_and_multiply(m, e, n, c);
}
