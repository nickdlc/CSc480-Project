//Group 2
//Fall 2021 
//CSc 480 
//Distributed Key Generation

#include <NTL/ZZ_pX.h>
#include <gmp.h>
#include <iostream>
#include <vector>
#include <map>

#include "gmp_defs.h"
#include "Party.h"
#include "Member.h"
#include "Function.h"


#include "ntl_functions.cpp"

using namespace NTL;
using namespace std;

/* global vars for params q,p,g,rand_state */
mpz_t param_q, param_p, param_g;
gmp_randstate_t rand_state;

bool primetest(mpz_t p) {
    while (gmp_scanf("%Zd",p) == 1) {
		if (ISPRIME(p)) return true;
	}

    return false;
}

void generate_params() {
    /* select random prime q of the right number of bits, then multiply
	 * by a random even integer, add 1, check if that is prime.  If so,
	 * we've found q and p respectively. */
	// const size_t qBitlen = 256;
	// const size_t pBitlen = 2048;
	const size_t qBitlen = 512;
	const size_t pBitlen = 4096;
	/* lengths in BYTES: */
	size_t qLen = qBitlen / 8 + (qBitlen % 8 != 0);
	size_t pLen = pBitlen / 8 + (pBitlen % 8 != 0);
	size_t rLen = pLen - qLen;
	unsigned char* qCand = (unsigned char*)malloc(qLen);
	unsigned char* rCand = (unsigned char*)malloc(rLen);
	NEWZ(q);
	NEWZ(p);
	NEWZ(r); /* holds (p-1)/q */
	NEWZ(t); /* scratch space */
	NEWZ(g); /* generator of subgroup */
	FILE* f = fopen("/dev/urandom","rb");
	do {
		do {
			fread(qCand,1,qLen,f);
			BYTES2Z(q,qCand,qLen);
		} while (!ISPRIME(q));
		/* now try to get p */
		fread(rCand,1,rLen,f);
		rCand[0] &= 0xfe; /* set least significant bit to 0 (make r even) */
		BYTES2Z(r,rCand,rLen);
		mpz_mul(p,q,r);     /* p = q*r */
		mpz_add_ui(p,p,1);  /* p = p+1 */
		/* should make sure q^2 doesn't divide p-1.
		 * suffices to check if q|r */
		mpz_mod(t,r,q);     /* t = r%q */
		/* now check if t is 0: */
		if (mpz_cmp_ui(t,0) == 0) continue; /* really unlucky! */
	} while (!ISPRIME(p));
	gmp_printf("q = %Zd\np = %Zd\n",q,p);
	/* now find a generator of the subgroup of order q.
	 * Turns out just about anything to the r power will work: */
	size_t tLen = qLen; /* qLen somewhat arbitrary. */
	unsigned char* tCand = (unsigned char*)malloc(tLen);
	do {
		fread(tCand,1,tLen,f);
		BYTES2Z(t,tCand,tLen);
		if (mpz_cmp_ui(t,0) == 0) continue; /* really unlucky! */
		mpz_powm(g,t,r,p); /* efficiently do g = t**r % p */
	} while (mpz_cmp_ui(g,1) == 0); /* since q prime, any such g /= 1
									   will actually be a generator of
									   the subgroup. */
	fclose(f);
	gmp_printf("g = %Zd\n",g);
    
    mpz_set(param_q, q);
    mpz_set(param_p, p);
    mpz_set(param_g, g);
	
	// Generate the random state for our random number generation
	gmp_randinit_default(rand_state);

}

int main(int argc, char* argv[]) {
    int n, t;
    // optional args n = $1, t = $2
    if (argc == 2) {
        n = stoi(argv[1]);
        t = n / 2; // threshold is default half of total members
    } else if (argc == 3) {
        n = stoi(argv[1]);
        t = stoi(argv[2]);
    } else {
        n = 14;
        t = 7;
    }
    
	vector<member> members_in_party_one = { };
    unsigned int size_of_party = 10;
    unsigned int size_of_threshold = 1;

    //uses the empty constructor to declare this object, will need to manually update the size_of_party, size_of_threshold, and all_members vector 
    
	party party1 = party(); 
    party1.set_party_size(size_of_party); //sets the size_of_party
    party1.set_threshold_size(size_of_threshold);//sets the size_of_threshold
    party1.set_members_and_party_threshold_sizes(); //creates the new members and updates those members size_of_party and size_of_threshold variables
	
	
	//Alternatively, one can uses the constructor that takes the 3 parameter below to declare this object and these variables will updated to both the party and members
    //party party1 = party(size_of_party, size_of_threshold, members_in_party_one);  

    readParams();
	initNTLRandom();

    ZZ_pX function;
           
    //generates random little polynomials for every member in party 1. I print f which will show all coefficients in the polynomial
    for(int i = 0; i < party1.size_of_party; i++){
        function = random_ZZ_pX(party1.size_of_party);
        party1.all_members[i].set_little_function(function);
        cout << "little function for member, " << party1.all_members[i].getIndex()  << ", is: \n" <<  party1.all_members[i].get_little_function()  << endl; 

     }
	//brings 3 coefficients from the little polynomial of the member with index 10 who is in party1.
    cout << "\nlittle function coefficient at index 0 for member " << party1.all_members[9].getIndex()  << ", is: \n" <<  party1.all_members[9].get_little_function()[0]  << endl; 
    cout << "\nlittle function coefficient at index 1 for member " << party1.all_members[9].getIndex()  << ", is: \n" <<  party1.all_members[9].get_little_function()[1]  << endl; 
    cout << "\nlittle function coefficient at index 2 for member " << party1.all_members[9].getIndex()  << ", is: \n" <<  party1.all_members[9].get_little_function()[2]  << endl; 

    //generate_params();

    return 0; 
}