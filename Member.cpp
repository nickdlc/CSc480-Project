#include "Member.h"

extern ZZ g;

member::member(unsigned int identity, unsigned int threshold)
{
    this->identity = identity;
    this->my_poly = random_ZZ_pX(threshold + 1);
    this->my_secret_key = eval(this->my_poly, ZZ_p(0));
    power(this->my_public_key, conv<ZZ_p>(g), conv<ZZ>(this->my_secret_key));
}

void member::set_my_share()
{
    for (int i = 0; i < this->points.size(); i++)
    {
        add(this->my_share, this->my_share, this->points[i]); // x = a + b
    }
}