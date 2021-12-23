#pragma once

#include <vector>
#include "ntl_gmp.h"
#include <numeric>

using namespace std;

extern ZZ q, p, g;

class member
{
public:
    unsigned int identity; // identity of a member
    ZZ_p my_public_key;    // public key of a member
    member(unsigned int identity, unsigned int threshold);
    ZZ_pX get_my_poly() { return my_poly; }
    ZZ_p get_my_secret_key() { return my_secret_key; }
    void set_is_dishonest() { this->is_dishonest = true; }
    bool get_is_dishonest() { return this->is_dishonest; }
    // public keys of all members
    vector<ZZ_p> public_keys;
    void receive_public_keys(ZZ_p public_key) { this->public_keys.push_back(public_key); }
    void receive_points(ZZ_p point) { this->points.push_back(point); }
    vector<ZZ_p> get_points() { return this->points; }
    void receive_coeffs(vector<ZZ_p> coeff) { this->coeffs.push_back(coeff); }
    vector<vector<ZZ_p> > get_coeffs() { return this->coeffs; }
    // all maksed coefficients
    vector<vector<ZZ_p> > coeffs;
    void set_my_share();
    ZZ_p get_my_share() { return this->my_share; }
    void set_my_coeff();
    // my masked coefficients
    vector<ZZ_p> my_coeff;
    ZZ_p coeff_prod(vector<ZZ_p> coeff, int identity);
    // complaints from other members
    int complaints = 0;
    void receive_complaints() { this->complaints++; }

private:
    // if true, send random value to the others instead of using my_poly(j)
    bool is_dishonest = false;

    // secret polynomial
    // evaluate my and other members' polynomials with this
    ZZ_pX my_poly;

    // my_poly evaluated at 0
    // sum of all members' keys = group secret key
    // use this to verify the group secret
    ZZ_p my_secret_key;

    // a point at group's secret polynomial
    // = sum of points
    ZZ_p my_share = conv<ZZ_p>(0);

    // value of my and other members' polynomials evaluated at my identity
    vector<ZZ_p> points;
};