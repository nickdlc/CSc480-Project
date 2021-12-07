//Chelsea Lantigua
//Fall 2021 
//CSc 480

#include <NTL/ZZ_pX.h>
#include <gmp.h>
#include <iostream>
#include <vector>
#include <map>

/* defines adapted from Piazza posts */
#define ISPRIME(x) mpz_probab_prime_p(x,10)
/* macro to declare a new long integer: */
#define NEWZ(x) mpz_t x; mpz_init(x)
/* these will read/write integers from byte arrays where the
 * least significant byte is first (little endian bytewise). */
#define BYTES2Z(x,buf,len) mpz_import(x,len,-1,1,0,0,buf)
#define Z2BYTES(buf,len,x) mpz_export(buf,&len,-1,1,0,0,x)

using namespace NTL;
using namespace std;

/* global vars for params q,p,g */
mpz_t param_q, param_p, param_g;

//represents a member in a party
class member{
    private:
        double secret_key; //sum of all the individual keys we make to distribute to others + ours
        float my_share; //one's own share of their little function evalutated at their index which is index = self.index - 1    
        float big_function_evaluated;//big function evaluated at our index
        map<member, float> big_function_evaluated_by_others; // stores the evaluation of the big function by the members in the group
        float big_secret; //our computation of the secret being communicated to the group 
        map<member, float> shares_for_others; //stores to then distribute all shares you compute for the other members in the group based on your little function.
        map<member, float> shares_from_others; //stores all shares the other members of the group will send you
        map<member, vector<float> > othersCoefficients; //store the coefficients vector of the others members for phase 2
        vector<float> my_coefficients; //stores the coefficients of the member's polynomial (little function) 


    public: 
        int index; //index of the member   
        float public_key; //g^secret_key
        vector<float> my_coefficients_to_g; //stores the coefficients of the member's polynomial (little function) as the exponent to generator g

    //constructor
    member(const int index){
        this->index = index;
    }; //constructed based on party and index of that member

    //copy constructor
    member(const member& m){
        this->index = m.index;
    }

    void setIndex(int i){
        index = i++; //index can not be 0
    }
    unsigned int getIndex(){
        return index;
    }

    void set_my_share(float share){
        my_share = share;
    } 

    //to get one's own share of the secret
    float get_my_share(){
        return my_share;
    }

    //generate a share for everyone else in the threshold -- excluding yourself
    float generate_shares();

    //refers to the other member objects in the party to set their shares_from_others map based on our shares_for_others map.
    void set_shares_for_others(){};

    //will update your shares_from_others map if verify_share is true else will broadcast a warning message about you.
    void update_all_shares(){}; //get an index 
        
    //sum of the shares recieved from other members little polynomials evaluated at your index plus your share which is your little polynomial evaluated at your index
    float generate_share_of_big_function(){
        int sum = my_share;
        return sum;
    }
 
    //need to store the other big_function_evaluated variable value of other members into this vector in order to eventually add these values to then decrypt to determine the secret message
    void set_big_function_evaluated_by_others(){} //To do

    //the big secret is the sum of all of the members in threshold group's big_function_evaluated value
    void compute_big_secret(){}//To do

   

    //for phase 2

    //return true if the share if we can successfully verify the share is correct returns false 
    bool consistency_check(); // To do
    
    //set the coefficients as exponents to base g
    void set_coefficient(){}; // To do

    //broadcast the coefficients to a members of the party
    void broadcast_coefficient(){}; // To do



};


//represents the party
class party{

    public: 
        int g; //generator g
        unsigned int size_of_party; // n 
        vector<member> all_members; //all members in the group
        unsigned int threshold; // size of t must be < n
        vector<member> members_in_threshold_group; //all members in the threshold group
        int public_key; //public key of the group will be the product of everyone's little function
        
        //constructor
        party(unsigned int size_of_party, unsigned int threshold){ //want to add vector of members in the group, all_members, in the constructor but I get errors when I create an object
            this->size_of_party = size_of_party;
            this->threshold = threshold;
        };
    
        //product of all of the members in threshold group's g^big_function_evaluated value
        void set_public_key(){
            for(int i = 0; i < members_in_threshold_group.size(); i++){
                public_key += members_in_threshold_group[i].public_key;
            }
        }

        float get_public_key(){
            return public_key;
        }
};

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

}

int main(){

    //testing    
    party party1(5, 2);
    member m(3);
    cout << "member number " << m.getIndex() << endl;
    cout << "party size of threshold " << party1.threshold << endl;

    generate_params();

    return 0; 
}