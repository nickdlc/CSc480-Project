#include "Member.h"

// constructor
member::member() {
    // Empty for now
}

member::member(int ind) { 
    index = ind;

}; //constructed based on party and index of that member

member::member(int ind, unsigned int party_size, unsigned int threshold_size) {
    this->index = ind;
    this->size_of_party = party_size;
    this->threshold = threshold_size;
    this->function = random_ZZ_pX(threshold_size);

}; //constructed based on party and index of that member

// copy constructor
member::member(const member& m) {
    this->index = m.index;
    this->deg = m.deg;
    this->function = ZZ_pX(m.function);
}

// Generate the shares for each member in the party
void member::generate_shares() {
    for (unsigned int i = 0; i < this->size_of_party; i++) {
        // Add a new share for each member i and evaluate our little function at i
        member_shares.insert(
            pair<member_index_t, ZZ_p>(i, 
                eval(this->function, ZZ_p(i))));
    }
}