#include "Party.h"

party::party(unsigned int num_all_members, unsigned int threshold)
{
    this->num_all_members = num_all_members;
    this->threshold = threshold;
    // add members to the party with only identity and threshold
    for (int i = 0; i < num_all_members; i++)
    {
        this->all_members.push_back(member(i, threshold));
    }
    // randomly pick size of participating party(< total members and >= threhold)
    this->num_participating_members = RandomBnd(num_all_members - threshold + 1) + threshold;
    // randomly drop residual members
    this->participating_members = this->all_members;
    int n = this->num_all_members - this->num_participating_members;
    for (int i = 0; i < n; i++)
    {
        // target = RandomBnd(size of vector);
        // delete by target index
        this->participating_members.erase(this->participating_members.begin() + RandomBnd(this->participating_members.size()));
    }
};