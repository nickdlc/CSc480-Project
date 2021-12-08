#pragma once

#include <vector>
#include "Member.h"

using namespace std;
//represents the party
class Party {

    public: 
        int g; //generator g
        unsigned int size_of_party; // n 
        vector<Member> all_members; //all members in the group
        unsigned int threshold; // size of t must be < n
        vector<Member> members_in_threshold_group; //all members in the threshold group
        int public_key; //public key of the group will be the product of everyone's little function
        
        //constructor
        Party(unsigned int size_of_party, unsigned int threshold) { //want to add vector of members in the group, all_members, in the constructor but I get errors when I create an object
            this->size_of_party = size_of_party;
            this->threshold = threshold;
        };
    
        //product of all of the members in threshold group's g^big_function_evaluated value
        void set_public_key() {
            for(int i = 0; i < members_in_threshold_group.size(); i++){
                public_key += members_in_threshold_group[i].public_key;
            }
        }

        float get_public_key() {
            return public_key;
        }
};