#include "Member.h"

// constructor
Member::Member(const int index, unsigned int deg) : index(index), deg(deg), my_function(Function(deg)) {
    //this->my_function = Function(this->deg);
}

// copy constructor
Member::Member(const Member& m) {
    this->index = m.index;
    this->deg = m.deg;
    this->my_function = Function(m.my_function);
}