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
    initNTLRandom();

    // initialize party with n members and t threshold
    party p = party(n, t);

    cout << "total member count = " << p.num_all_members << endl;
    cout << "pariticipating member count = " << p.num_participating_members << endl;
    cout << "threshold = " << p.threshold << "\n\n";
    cout << "total members,\n";

    // for (int i = 0; i < p.num_all_members; i++)
    // {
    //     cout << "members[" << i << "]" << endl;
    // }

    cout << "\nparticipating members,\n";

    for (int i = 0; i < p.num_participating_members; i++)
    {
        cout << "members[" << p.participating_members[i].identity << "]" << endl;
    }

    // for (int i = 0; i < p.num_participating_members; i++)
    // {
    //     cout << "\nparticipating_members[" << p.participating_members[i].identity << "].coeffs,\n";
    //     for (int j = 0; j < p.num_participating_members; j++)
    //     {
    //         cout << "\nmember[" << p.participating_members[j].identity << "].my_coeff,\n";
    //         for (int k = 0; (k < (deg(p.participating_members[j].get_my_poly()) + 1)); k++)
    //         {
    //             cout << p.participating_members[i].coeffs[j][k] << endl;
    //         }
    //     }
    // }

    // for (int i = 0; i < p.num_participating_members; i++)
    // {
    //     cout << "\nparticipating_members[" << p.participating_members[i].identity << "].points,\n";
    //     vector<ZZ_p> points = p.participating_members[i].get_points();
    //     for (int j = 0; j < points.size(); j++)
    //         cout << points[j] << endl;
    // }

    // for (int i = 0; i < p.num_participating_members; i++)
    // {
    //     cout << "\nparticipating_members[" << p.participating_members[i].identity << "].public_keys,\n";
    //     for (int j = 0; j < p.num_participating_members; j++)
    //         cout << p.participating_members[i].public_keys[j] << "\n";
    // }

    for (int i = 0; i < p.num_participating_members; i++)
    {
        for (int j = 0; j < p.num_participating_members; j++)
        {

            if (i == 0)
                break;

            cout << "\nmember[" << p.participating_members[i].identity << "] tests member[" << p.participating_members[j].identity << "]\n";
            ZZ_p a = power(conv<ZZ_p>(g), conv<ZZ>(p.participating_members[i].get_points()[j]));
            vector<ZZ_p> coeff = p.participating_members[i].coeffs[j];
            int identity = p.participating_members[i].identity;
            ZZ_p b = ZZ_p(1);
            for (int k = 0; k < p.threshold + 1; k++)
            {
                // cout << identity << "^" << k << " = " << power(ZZ(identity), long(k)) << endl;
                // cout << coeff[k] << endl;
                mul(b, b, power(coeff[k], power(ZZ(identity), long(k))));
            }
            cout << "a = " << a << "\n";
            cout << "b = " << b << "\n\n";

            // if (a != b)
            // {
            //     cout << "test failed, sending complaint\n";
            //     p.participating_members[j].receive_complaints();
            // }
        }
    }

    vector<member> tmp = p.get_dishonest_members();

    cout << "\ndishonest member count = " << p.num_dishonest_members << "\n\n";
    cout << "dishonest members,\n";
    for (int i = 0; i < p.num_dishonest_members; i++)
    {
        cout << "dishonest_members[" << tmp[i].identity << "]" << endl;
    }

    cout << "\n";

    for (int i = 0; i < p.num_participating_members; i++)
    {
        cout << "member[" << p.participating_members[i].identity << "].is_dishonest = " << p.participating_members[i].get_is_dishonest() << "\n";
        cout << "member[" << p.participating_members[i].identity << "].complaints = " << p.participating_members[i].complaints << "\n\n";
    }

    for (int i = 0; i < p.num_participating_members; i++)
    {
        cout << "member[" << p.participating_members[i].identity << "].my_share = " << p.participating_members[i].get_my_share() << endl;
    }

    // cout << "\ngroup_public_key = " << p.group_public_key << endl;

    // cout << "\ngroup_secret_key = " << p.get_group_secret_key() << endl;

    return 0;
}