#pragma once

#include <vector>
#include <map>

using namespace std;

//represents a member in a party
class Member {
    private:
        double secret_key; //sum of all the individual keys we make to distribute to others + ours
        float my_share; //one's own share of their little function evalutated at their index which is index = self.index - 1    
        float big_function_evaluated;//big function evaluated at our index
        map<Member, float> big_function_evaluated_by_others; // stores the evaluation of the big function by the members in the group
        float big_secret; //our computation of the secret being communicated to the group 
        map<Member, float> shares_for_others; //stores to then distribute all shares you compute for the other members in the group based on your little function.
        map<Member, float> shares_from_others; //stores all shares the other members of the group will send you
        map<Member, vector<float> > othersCoefficients; //store the coefficients vector of the others members for phase 2
        vector<float> my_coefficients; //stores the coefficients of the member's polynomial (little function) 


    public: 
        int index; //index of the member   
        float public_key; //g^secret_key
        vector<float> my_coefficients_to_g; //stores the coefficients of the member's polynomial (little function) as the exponent to generator g

    //constructor
    Member(const int index) {
        this->index = index;
    }; //constructed based on party and index of that member

    //copy constructor
    Member(const Member& m) {
        this->index = m.index;
    }

    void setIndex(int i) {
        index = i++; //index can not be 0
    }
    unsigned int getIndex() {
        return index;
    }

    void set_my_share(float share) {
        my_share = share;
    } 

    //to get one's own share of the secret
    float get_my_share() {
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