#pragma once

#include <vector>
#include <gmp.h>
#include "gmp_defs.h"

using namespace std;

class Function {
    public:
        Function(unsigned int deg);
        Function(const Function& f);
        
        coefficients_t get_coeffs() {
            // res.clear();
            // for (int i = 0; i < this->deg + 1; i++) {
            //     res.push_back(this->coeffs[i]);
            // }
            return coeffs;
        }
        coefficients_t get_generators() { return generators; }
        
        void evaluate_f(mpz_t res, unsigned int x);
        void get_secret(mpz_t res);
        
        ~Function();
    private:
        unsigned int deg;
        coefficients_t coeffs;
        coefficients_t generators;
};