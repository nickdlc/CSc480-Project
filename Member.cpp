#include "Member.h"

member::member(unsigned int identity, unsigned int threshold)
{
    this->identity = identity;
    this->my_poly = random_ZZ_pX(threshold + 1);
    this->my_secret_key = eval(this->my_poly, ZZ_p(0));
    // this->my_public_key = power(g, this->my_secret_key);
    // this->my_points;     // need exchange
    // this->my_share;      // need exchange
    // this->other_share;   // need exchange
}
