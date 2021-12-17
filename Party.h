#pragma once

#include "Member.h"

class party
{
public:
    unsigned int num_all_members;           // # of all members
    unsigned int threshold;                 // threshold for the key generatrion
    vector<member> all_members;             // array of all members
    unsigned int num_participating_members; // # of participating members(>=threshold)
    vector<member> participating_members;   // array of participating members
    ZZ_p group_public_key;                  // public key of the group
    party(unsigned int num_all_members, unsigned int threshold);

private:
    ZZ_p group_secret_key; // secret key of the group
};
// change the new_Party.cpp to Party.cpp when finished