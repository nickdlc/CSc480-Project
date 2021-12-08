#include "Function.h"

extern mpz_t param_g, param_q;
extern gmp_randstate_t rand_state;

Function::Function(unsigned int deg) {
    // Generate the random coefficients for our polynomial of degree deg
    for (int i = 0; i < deg + 1; i++) {
        NEWZ(coeff);
        // Coefficients are 32-bit
        mpz_urandomb(coeff, rand_state, 32);
        this->coeffs.push_back(&coeff);
        
        NEWZ(g_coeff);
        mpz_powm(g_coeff, param_g, coeff, param_q);
        this->generators.push_back(&g_coeff);
    }
}

void Function::evaluate_f(mpz_t res, unsigned int x) {
    NEWZ(x_mpz);
    mpz_init_set_ui(x_mpz, x);
    
    NEWZ(sum);
    NEWZ(intermediate);
    for (int i = 0; i < this->deg + 1; i++) {
        mpz_t coeff_i = this->coeffs[i];
        mpz_pow_ui(intermediate, x_mpz, i);
        mpz_mul(intermediate, coeff_i, intermediate);
        mpz_add(sum, sum, intermediate);
    }
    mpz_clear(intermediate);
    mpz_clear(x_mpz);
    mpz_mod(sum, sum, param_q);
    
    mpz_set(res, sum);
}

void Fucntion::get_secret(mpz_t res) { return evaluate_f(res, 0); }

Function::~Function() {
    for (int i = 0; i < this->deg + 1; i++) {
        mpz_clear(*(this->coeffs[i]));
        mpz_clear(*(this->coeffs[i]));
    }
}