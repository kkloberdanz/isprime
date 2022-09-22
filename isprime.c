#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gmp.h>

static bool thorough_is_prime(mpz_t n) {
    mpz_t root;
    mpz_t i;
    mpz_t out;
    bool result = true;

    if (mpz_cmp_ui(n, 2) == 0) {
        return true;
    } else if (mpz_cmp_ui(n, 2) < 0) {
        return false;
    }

    mpz_init(out);

    /* check if divisible by 2 */
    mpz_mod_ui(out, n, 2);
    if (mpz_cmp_ui(out, 0) == 0) {
        result = false;
        goto clear_out;
    }

    mpz_init(root);
    mpz_init(i);

    mpz_sqrt(root, n);
    mpz_add_ui(root, root, 1);

    /* TODO: multi-thread */
    for (mpz_set_ui(i, 3); (mpz_cmp(i, root) < 0); mpz_add_ui(i, i, 2)) {
        mpz_mod(out, n, i);
        if (mpz_cmp_ui(out, 0) == 0) {
            result = false;
            goto clear_all;
        }
    }

clear_all:
    mpz_clear(root);
    mpz_clear(i);
clear_out:
    mpz_clear(out);
    return result;
}

static bool isprime(mpz_t n) {
    int miller_rabin;

    miller_rabin = mpz_probab_prime_p(n, 50);
    switch (miller_rabin) {
    case 0:
        /* definitely not prime */
        return false;
    case 2:
        /* definitely prime */
        return true;
    default:
        /*
         * If neither 2 nor 0, then it is probably prime, but not certain.
         * Check it exhaustively.
         */
        mpz_out_str(stdout, 10, n);
        printf(" might be prime (thinking)...\n");
        fflush(stdout);
        return thorough_is_prime(n);
    }
}

int main(void) {
    mpz_t n;
    bool result;

    mpz_init(n);
    for (;;) {
        mpz_inp_str(n, stdin, 10);
        if (mpz_cmp_ui(n, 0) == 0) {
            break;
        }
        result = isprime(n);
        mpz_out_str(stdout, 10, n);
        if (result) {
            puts(" is prime");
        } else {
            puts(" is NOT prime");
        }
        mpz_set_ui(n, 0);
    }
    mpz_clear(n);
    return 0;
}
