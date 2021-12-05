//Chelsea Lantigua 
//Fall 2021 
//CSc 480
//Initial commit - I constructed two classes a member class and a party class. 
//I developed a few methods and declared several other methods that we should to develop.

#include <iostream>
#include <vector>
#include <map>

using namespace std;

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
    member(int index){
        this->index = index;
    }; //constructed based on party and index of that member

    //copy constructor
    member(member& m){
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


int main(){

    //testing    
    party party1(5, 2);
    member m(3);
    cout << "member number " << m.getIndex() << endl;
    cout << "party size of threshold " << party1.threshold << endl;

    return 0; 
}