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
        if (n < 2)
        {
            perror("you need at least 2 members for a secret sharing");
            return -1;
        }
        /*––––––––––––– do we need a constraint n>=2? –––––––––––––*/
        t = n / 2; // threshold is default half of total members
    }
    else if (argc == 3)
    {
        n = stoi(argv[1]);
        if (n < 2)
        {
            perror("you need at least 2 members for a secret sharing");
            return -1;
        }
        t = stoi(argv[2]);
        if (t > n)
        {
            perror("threshold value should be smaller than or equal to the size of the party");
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

    // cout << "total member count = " << p.all_members.size() << endl;
    // cout << "participating member count = " << p.participating_members.size() << "\n\n";
    // cout << "total members,\n";

    // for (int i = 0; i < p.all_members.size(); i++)
    // {
    //     cout << "members[" << i << "]" << endl;
    //     // cout << "members[" << i << "].get_mypoly() = " << p.all_members[i].get_my_poly() << endl;
    // }

    // cout << "\nparticipating members,\n";

    // for (int i = 0; i < p.participating_members.size(); i++)
    // {
    //     cout << "members[" << p.participating_members[i].identity << "]" << endl;
    //     // cout << "participating_members[" << i << "]'s polynomial = " << p.participating_members[i].get_my_poly() << endl;
    // }

    // cout << "my_poly = " << p.participating_members[0].get_my_poly() << endl;
    // cout << "my_secret_key = " << p.participating_members[0].get_my_secret_key() << endl;
    // cout << "my_public_key = " << p.participating_members[0].my_public_key << endl;

    // vector<member> tmp = p.get_dishonest_members();

    // cout << "\ndishonest member count = " << p.num_dishonest_members << "\n\n";
    // cout << "dishonest members,\n";
    // for (int i = 0; i < p.num_dishonest_members; i++)
    // {
    //     cout << "dishonest_members[" << tmp[i].identity << "]" << endl;
    // }

    // cout << "\n";

    // for (int i = 0; i < p.num_participating_members; i++)
    // {
    //     cout << "member[" << p.participating_members[i].identity << "].is_dishonest = " << p.participating_members[i].get_is_dishonest() << endl;
    // }

    for (int i = 0; i < p.num_participating_members; i++)
    {
        cout << "member[" << p.participating_members[i].identity << "].my_share = " << p.participating_members[i].get_my_share() << endl;
    }

    cout << "\ngroup_public_key = " << p.group_public_key << endl;

    return 0;
}