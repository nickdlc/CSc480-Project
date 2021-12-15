#pragma once

#include <NTL/ZZ_pX.h>
#include <vector>
#include "Member.h"
#include <iostream>
using namespace std;

// These are the global q, p, and g variables from NTL
extern ZZ q, p, g;

//represents the party
class party
{

public:
    unsigned int size_of_party; // n

    // This class member should be accessed by all members to see the
    static vector<member> all_members; //all members in the group

    unsigned int threshold;                    // size of t must be < n
    vector<member> members_in_threshold_group; //all members in the threshold group
    ZZ_p public_key;                           //public key of the group will be the product of everyone's little function

    // Constructors located in the implementation file Party.cpp
    //constructor

    party();

    party(unsigned int size_of_party, unsigned int threshold, vector<member> all_members);

    //copy constructor
    party(const party &p);

    void set_public_key(){};

    ZZ_p get_public_key()
    {
        return public_key;
    }

    //to set the party's size variable
    void set_party_size(unsigned int size)
    {
        this->size_of_party = size;
    }

    //to set the party's threshold size variable
    void set_threshold_size(unsigned int size)
    {
        this->threshold = size;
    }
    //will update this->all.members vector to the members in the input vector
    void set_party_members(vector<member> members)
    {
        for (int i = 0; i < members.size(); i++)
        {
            this->all_members.push_back(members[i]);
        }
    }
    //will print the index variable value of the member
    void print_member_index()
    {
        for (int i = 0; i < this->size_of_party; i++)
        {
            cout << "party member " << all_members[i].getIndex() << endl;
        }
    }
    //this method will create new members for the party their indicies will be 1 up to the size of the party,
    //and set the members's threshold and party size variables according to the party's variables.
    //Finally it will update the party's all_members vector.
    void set_members_and_party_threshold_sizes()
    {
        vector<member> members_in_party = {};
        for (int i = 0; i < this->size_of_party; i++)
        {
            member m = member(i + 1);
            members_in_party.push_back(m);
            members_in_party[i].size_of_party = this->size_of_party; //sets the size_of_party variable for the member
            members_in_party[i].threshold = this->threshold;         //sets the threshold variable for the member
            cout << "member " << members_in_party[i].getIndex() << "th updated party size : " << members_in_party[i].get_party_size() << endl;
            cout << "member " << members_in_party[i].getIndex() << "th updated threshold size : " << members_in_party[i].get_threshold_size() << endl;
        }
        set_party_members(members_in_party); //to update the all_members vector
    }
    // 12/15 Jae
    // each member exchanges s_i evaluated at j
    // void exchange_sij()
    // {
    //     for (int i = 0; i < this->members_in_threshold_group.size(); i++)
    //     {
    //         // preparing argument for recieve_share
    //         const member *m = this->members_in_threshold_group[i];
    //         const ZZ_p share = this->members_in_threshold_group[i];

    //         // need to find out how to evaluate the share at j

    //         for (int j = 0; j < this->members_in_threshold_group.size(); j++)
    //         {
    //             // i is the sender and j is the reciever
    //             if (j != i)
    //             {
    //                 this->members_in_threshold_group[j].recieve_share(&m, share);
    //                 // possibly exchange public key here
    //             }
    //         }
    //     }
    // }
};
