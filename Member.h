#pragma once

#include <vector>
#include "ntl_gmp.h"

using namespace std;

extern ZZ q, p, g;

class member
{
public:
    unsigned int identity; // identity of a member
    ZZ_p my_public_key;    // public key of a member
    // initialzie members in a for loop
    member(unsigned int identity, unsigned int threshold);
    ZZ_pX get_my_poly() { return my_poly; }
    ZZ_p get_my_secret_key() { return my_secret_key; }

private:
    // secret polynomial
    // evaluate other members' polynomials with this
    ZZ_pX my_poly;

    // my_poly evaluated at 0
    // sum of all members' keys = group secret key
    // use this to verify the group secret
    ZZ_p my_secret_key;

    // value of other members' polynomials evaluated at my identity
    ZZ_p my_points;

    // a point at group's secret polynomial
    // = sum of my points and my polynomial evaluated my identity
    ZZ_p my_share;

    // values of my polynomial evaulted at other members' identity
    // the member can maliciously send the wrong values
    vector<ZZ_p> other_share;

    // // vector of my coefficients
    // vector<ZZ_p> my_coeff;
};