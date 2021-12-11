#include "Member.h"
#include "Party.h"

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
    for (const member m : party::all_members) {
        // Add a new share for each member i and evaluate our little function at i
        this->shares_for_others.insert(
            make_pair(&m, eval(this->function, ZZ_p(m.index))));
    }
}

// This member gets a share from another member and sets it as the share recieved from them
void member::recieve_share(const member *m, const ZZ_p share) const {
    this->shares_from_others.insert(make_pair(m, share));
}

void member::set_shares_for_others() {
    for (const member m : party::all_members) {
        m.recieve_share(this, this->shares_for_others.at(&m));
    }
}