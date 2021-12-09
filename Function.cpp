#include "Function.h"

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

Function::Function() {
    
}

Function::Function(const Function& f) {
    for (int i = 0; i < f.deg + 1; i++) {
        NEWZ(coeff);
        mpz_set(coeff, *f.coeffs[i]);
        this->coeffs.push_back(&coeff);

        NEWZ(g_coeff);
        mpz_set(g_coeff, *f.generators[i]);
        this->generators.push_back(&g_coeff);
    }
}

void Function::evaluate_f(mpz_t res, unsigned int x) {
    /* @param res: initial mpz_t value which is used to store the result
     * @param x: unsigned int to evaluate f at
     *
     * Computes f(x) and stores the result in res */
    
    // Convert x to mpz_t
    NEWZ(x_mpz);
    mpz_init_set_ui(x_mpz, x);
    
    NEWZ(sum);
    NEWZ(intermediate);
    for (int i = 0; i < this->deg + 1; i++) {
        NEWZ(coeff_i);
        mpz_set(coeff_i, *this->coeffs[i]);
        mpz_pow_ui(intermediate, x_mpz, i);
        mpz_mul(intermediate, coeff_i, intermediate);
        mpz_add(sum, sum, intermediate);
    }

    // Free space for intermediate and x_mpz
    mpz_clear(intermediate);
    mpz_clear(x_mpz);
    mpz_mod(sum, sum, param_q);
    
    mpz_set(res, sum);
}

void Function::get_secret(mpz_t res) {
    /* @param res: initial mpz_t value which is used to store the secret f(0)
     *
     * Stores f(0) in res */
    
    evaluate_f(res, 0);
}

Function::~Function() {
    if (this->coeffs.empty()) {
        for (int i = 0; i < this->deg + 1; i++) {
            // Free space for each coefficient and generator g
            mpz_clear(*(this->coeffs[i]));
            mpz_clear(*(this->generators[i]));
        }
    }
}