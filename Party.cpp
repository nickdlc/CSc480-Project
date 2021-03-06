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
    // this->group_set_shares();
    // this->set_group_secret_key();
    // this->set_group_public_key();
    // this->complain();
};

// add all members to the party
void party::set_all_members()
{
    // add members to the party with only identity and threshold
    for (int i = 0; i < num_all_members; i++)
        this->all_members.push_back(member(i, threshold));
    // this->set_participating_members();
}
// choose n, threshold >= n, from the party
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
    this->num_dishonest_members = 0; // ????????????????????????????????????????????????delete this line after testing???????????????????????????????????????????????? //
    vector<int> indexes = choose_n_indexes(this->num_participating_members, this->num_dishonest_members);
    for (int i = 0; i < this->num_dishonest_members; i++)
    {
        int target = indexes[i];
        this->participating_members[target].set_is_dishonest();
        this->dishonest_members.push_back(this->participating_members[target]);
    }
}

// // store each members masked coefficient in one vector
// void party::set_coeffs()
// {
//     for (int i = 0; i < this->num_participating_members; i++)
//     {
//         this->coeffs.push_back(participating_members[i].my_coeff);
//     }
// }

// members exchange points, public keys
// exchanging coefficients does not happen here but in set_coeffs()
void party::exchange()
{
    // i is the sender and j is the receiver
    for (int i = 0; i < this->num_participating_members; i++)
    {
        for (int j = 0; j < this->num_participating_members; j++)
        {
            // if dishonest member, send random point
            if (participating_members[i].get_is_dishonest() == true)
            {
                // randomly choose victims not everyone is victim
                int dishonest = RandomBnd(2);
                if ((dishonest == 1) && (i != j))
                    participating_members[j].receive_points(random_ZZ_p());
                else
                    participating_members[j].receive_points(eval(participating_members[i].get_my_poly(), ZZ_p(participating_members[j].identity)));
            }
            // if honest member, send my_poly evaluated at identity
            else
                participating_members[j].receive_points(eval(participating_members[i].get_my_poly(), ZZ_p(participating_members[j].identity)));
            participating_members[j].receive_public_keys(participating_members[i].my_public_key);
            participating_members[j].receive_coeffs(participating_members[i].my_coeff);
        }
    }
}

// each member computes their shares, which are the points in big polynomial
void party::group_set_shares()
{
    for (int i = 0; i < this->num_participating_members; i++)
        participating_members[i].set_my_share();
}

// set group public key
void party::set_group_public_key()
{
    for (int i = 0; i < this->num_participating_members; i++)
        mul(this->group_public_key, this->group_public_key, participating_members[i].my_public_key);
}

// set group secret key
void party::set_group_secret_key()
{
    const size_t n = this->num_participating_members;

    // initialize vector of participating member identities
    vec_ZZ_p A;
    A.SetLength(n);

    // initialize matrix of participating member shares
    mat_ZZ_p B;
    B.SetDims(n,1);

    for (size_t i = 0; i < n; i++) {
        A[i] = ZZ_p(participating_members[i].identity);
        B[i][0] = participating_members[i].get_my_share();
    }

    // create vandermonde matrix for participating players
    mat_ZZ_p V = vandermonde(A);

    // initialize matrix of coefficients of F
    mat_ZZ_p C;

    // VC = B => C = V^{-1}B
    mul(C, inv(V), B);

    ZZ_pX F;
    F.SetLength(n);
    for (size_t i = 0; i < n; i++) {
        SetCoeff(F, i, C[i][0]);
    }

    // s = F(0)
    eval(this->group_secret_key, F, ZZ_p(0));
}

// complain dishonest members
void party::complain()
{
    // i tests j
    for (int i = 0; i < this->num_participating_members; i++)
    {
        for (int j = 0; j < this->num_participating_members; j++)
        {
            ZZ_p a = power(conv<ZZ_p>(g), conv<ZZ>(this->participating_members[i].get_points()[j]));
            // preparing the arguments for coeff_prod
            vector<ZZ_p> coeff = this->participating_members[i].coeffs[j];
            int identity = this->participating_members[i].identity;
            ZZ_p b = this->participating_members[i].coeff_prod(coeff, identity);
            if (a != b)
                participating_members[j].receive_complaints();
        }
    }
}