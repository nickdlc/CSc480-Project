#pragma once

#include "Member.h"

vector<int> choose_n_indexes(int arr_size, int n);

class party
{
public:
    // # of all members
    unsigned int num_all_members;
    // threshold for the key generatrion
    unsigned int threshold;
    // vector of all members
    vector<member> all_members;
    // # of participating members(>=threshold)
    unsigned int num_participating_members;
    // vector of participating members
    vector<member> participating_members;
    // public key of the group
    ZZ_p group_public_key = conv<ZZ_p>(1);
    // # of dishonest members
    unsigned int num_dishonest_members;
    party(unsigned int num_all_members, unsigned int threshold);
    vector<member> get_dishonest_members() { return this->dishonest_members; }
    void set_group_public_key();
    mat_ZZ_p get_group_secret_key() { return this->group_secret_key; }
    void complain();
    // vector of 'masked' coefficients of all members
    // vector<vector<ZZ_p> > coeffs;
    // void set_coeffs();

private:
    // secret key of the group
    mat_ZZ_p group_secret_key;
    void set_participating_members();
    void set_all_members();
    void set_dishonest_members();
    vector<member> dishonest_members;
    void exchange();
    void group_set_shares();
    void set_group_secret_key();
};