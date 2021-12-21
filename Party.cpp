#include "Party.h"

party::party(unsigned int num_all_members, unsigned int threshold)
{
    this->num_all_members = num_all_members;
    this->threshold = threshold;
    this->set_all_members();
    this->set_participating_members();
    this->num_participating_members = this->participating_members.size();
    this->set_dishonest_members();
    this->exchange();
    this->set_big_points();
    this->set_group_secret_key();
    this->set_group_public_key();
};

void party::set_all_members()
{
    // add members to the party with only identity and threshold
    for (int i = 0; i < num_all_members; i++)
    {
        this->all_members.push_back(member(i, threshold));
    }
    this->set_participating_members();
}

void party::set_participating_members()
{
    // randomly pick size of participating party(< total members and >= threhold)
    this->num_participating_members = RandomBnd(num_all_members - threshold + 1) + threshold;
    // randomly drop residual members
    this->participating_members = this->all_members;
    int num_members_to_exclude = this->num_all_members - this->num_participating_members;
    for (int i = 0; i < num_members_to_exclude; i++)
    {
        int target = RandomBnd(this->num_participating_members);
        this->participating_members.erase(this->participating_members.begin() + target);
    }
}

// return a vector of randomly chosen n indexes from a vector of size vector_size
vector<int> choose_n_indexes(int vector_size, int n)
{
    // creating a vector of indexes [0, ... , n - 1]
    vector<int> indexes(vector_size);
    iota(begin(indexes), end(indexes), 0);

    for (int i = 0; i < (vector_size - n); i++)
    {
        int target = RandomBnd(indexes.size());
        indexes.erase(indexes.begin() + target);
    }
    return indexes;
}

void party::set_dishonest_members()
{
    // everyone could be dishonest, but we enforce at least 2 members are honest
    this->num_dishonest_members = RandomBnd(this->num_participating_members - 1);
    vector<int> indexes = choose_n_indexes(this->num_participating_members, this->num_dishonest_members);
    for (int i = 0; i < this->num_dishonest_members; i++)
    {
        int target = indexes[i];
        this->participating_members[target].set_is_dishonest();
        this->dishonest_members.push_back(this->participating_members[target]);
    }
}

void party::exchange()
{
    for (int i = 0; i < this->num_participating_members; i++)
    {
        for (int j = 0; j < this->num_participating_members; j++)
        {
            // i is the sender and j is the receiver
            participating_members[j].receive_points(eval(participating_members[i].get_my_poly(), ZZ_p(participating_members[j].identity)));
            participating_members[j].receive_public_keys(participating_members[i].my_public_key);
            // participating_members[j].receive_coeff();
        }
    }
}

void party::set_big_points()
{
    for (int i = 0; i < this->num_participating_members; i++)
    {
        participating_members[i].set_my_share();
    }
}

void party::set_group_public_key()
{
    for (int i = 0; i < this->num_participating_members; i++)
    {
        add(this->group_public_key, this->group_public_key, participating_members[i].my_public_key);
    }
}

void party::set_group_secret_key()
{
    const size_t n = this->num_participating_members;
    vec_ZZ_p A;
    A.SetLength(n);
    for (size_t i = 0; i < n; i++)
    {
        A[i] = participating_members[i].get_my_share();
    }
    mat_ZZ_p V = vandermonde(A);
    this->group_secret_key = inv(V);
}