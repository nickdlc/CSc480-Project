#include "Member.h"

extern ZZ q, p, g;

member::member(unsigned int identity, unsigned int threshold)
{
    this->identity = identity;
    this->my_poly = random_ZZ_pX(threshold + 1);
    this->set_my_coeff();
    this->my_secret_key = eval(this->my_poly, ZZ_p(0));
    // setting my_public_key = g^(my_secret_key)
    power(this->my_public_key, conv<ZZ_p>(g), conv<ZZ>(this->my_secret_key));
}

void member::set_my_share()
{
    // setting my_share = sum of polynomials evaluated at my identity
    for (int i = 0; i < this->points.size(); i++)
        add(this->my_share, this->my_share, this->points[i]);
}
void member::set_my_coeff()
{
    // setting my_coeff = vector of g^(actual coefficients)
    for (int i = 0; i < (deg(this->my_poly) + 1); i++)
        this->my_coeff.push_back(power(conv<ZZ_p>(g), conv<ZZ>(this->my_poly[i])));
}

// computing sum of c_(ik) * j^k, pdf page 2 near the bottom
ZZ_p member::coeff_sum(vector<ZZ_p> coeff, int degree, int identity)
{
    ZZ_p result = conv<ZZ_p>(0);
    ZZ_p term;
    for (int i = 0; i < degree + 1; i++)
    {
        mul(term, coeff[i], power(conv<ZZ_p>(identity), conv<ZZ>(i)));
        add(result, result, term);
    }
    return result;
}
