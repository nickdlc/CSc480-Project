// Group 2
// Fall 2021
// CSc 480
// Distributed Key Generation

// #include <NTL/ZZ_pX.h>
#include <iostream>
#include <vector>
#include <map>
#include "ntl_gmp.h"
#include "Party.h"

using namespace NTL;
using namespace std;

/* GLOBAL VARIABLES */
//

/* global vars for params q,p,g,rand_state */
extern ZZ q, p, g;

/* global vars for member list */
// party all_nodes;

/* END GLOBAL VARIABLES */

int main(int argc, char *argv[])
{
    // Initialize n (total members playing) and t (threshold)
    int n, t;
    // optional args n = $1, t = $2
    if (argc == 2)
    {
        n = stoi(argv[1]);
        t = n / 2; // threshold is default half of total members
    }
    else if (argc == 3)
    {
        n = stoi(argv[1]);
        t = stoi(argv[2]);
        if (t >= n)
        {
            perror("threshold value should be smaller than the size of the party");
            return -1;
        }
    }
    else
    {
        n = 10;
        t = 5;
    }

    // initialize p, q, and g and set ZZ_p to q
    genParams();
    readParams();
    // cout << "q = " << q << endl;
    // cout << "p = " << p << endl;
    // cout << "g = " << g << endl;
    initNTLRandom();

    // initialize party with n members and t threshold
    party p = party(n, t);

    cout << "total member count = " << p.all_members.size() << endl;
    cout << "participating member count = " << p.participating_members.size() << endl;

    for (int i = 0; i < p.all_members.size(); i++)
    {
        cout << "all_members[" << i << "]'s identity = " << p.all_members[i].identity << endl;
        cout << "all_members[" << i << "]'s polynomial = " << p.all_members[i].get_my_poly() << endl;
    }

    for (int i = 0; i < p.participating_members.size(); i++)
    {
        cout << "participating_members[" << i << "]'s identity = " << p.participating_members[i].identity << endl;
        cout << "participating_members[" << i << "]'s polynomial = " << p.participating_members[i].get_my_poly() << endl;
    }

    cout << "my_poly = " << p.participating_members[0].get_my_poly() << endl;
    cout << "my_secret_key = " << p.participating_members[0].get_my_secret_key() << endl;
    cout << "my_public_key = " << p.participating_members[0].my_public_key << endl;

    // ------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------

    // vector<member> members_in_party_one = {};
    // unsigned int size_of_party = 10;
    // unsigned int size_of_threshold = 1;

    // //uses the empty constructor to declare this object, will need to manually update the size_of_party, size_of_threshold, and all_members vector

    // party party1 = party();
    // party1.set_party_size(size_of_party);           //sets the size_of_party
    // party1.set_threshold_size(size_of_threshold);   //sets the size_of_threshold
    // party1.set_members_and_party_threshold_sizes(); //creates the new members and updates those members size_of_party and size_of_threshold variables

    // //Alternatively, one can uses the constructor that takes the 3 parameter below to declare this object and these variables will updated to both the party and members
    // //party party1 = party(size_of_party, size_of_threshold, members_in_party_one);

    // ZZ_pX function;

    // //generates random little polynomials for every member in party 1. I print f which will show all coefficients in the polynomial
    // for (int i = 0; i < party1.size_of_party; i++)
    // {
    //     function = random_ZZ_pX(party1.size_of_party);
    //     party1.all_members[i].set_little_function(function);
    //     cout << "little function for member, " << party1.all_members[i].getIndex() << ", is: \n"
    //          << party1.all_members[i].get_little_function() << endl;
    // }

    // //brings 3 coefficients from the little polynomial of the member with index 10 who is in party1.
    // cout << "\nlittle function coefficient at index 0 for member " << party1.all_members[9].getIndex() << ", is: \n"
    //      << party1.all_members[9].get_little_function()[0] << endl;
    // cout << "\nlittle function coefficient at index 1 for member " << party1.all_members[9].getIndex() << ", is: \n"
    //      << party1.all_members[9].get_little_function()[1] << endl;
    // cout << "\nlittle function coefficient at index 2 for member " << party1.all_members[9].getIndex() << ", is: \n"
    //      << party1.all_members[9].get_little_function()[2] << endl;

    // //generate_params();

    // //cout << "Create party of " << n << "players with threshold " << t << "." << endl;
    // //Party party1(n, t);
    // //Member m(3, t);
    // //cout << "member number " << m.getIndex() << endl;
    // //cout << "party size of threshold " << party1.threshold << endl;

    return 0;
}