#include "Party.h"
//constructor

vector<member> party::all_members = vector<member>();

party::party(){};

party::party(unsigned int size_of_party, unsigned int threshold, vector<member> all_members)
{
    this->size_of_party = size_of_party;
    this->threshold = threshold;
    set_party_members(all_members); //to update the all_members vector
    for (int i = 0; i < all_members.size(); i++)
    {
        this->all_members[i].size_of_party = this->size_of_party; //sets the size_of_party variable of the member
        this->all_members[i].threshold = this->threshold;         //sets the threshold variable of the member
        // 12/15 Jae
        // this->all_members[i].setIndex(i)                          // set index of the member
    }
};

//copy constructor
party::party(const party &p)
{
    this->size_of_party = p.size_of_party;
    this->threshold = p.threshold;
    for (int i = 0; i < this->size_of_party; i++)
    {
        this->all_members[i] = p.all_members[i];
    }
}