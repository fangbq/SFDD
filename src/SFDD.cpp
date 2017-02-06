#include "SFDD.h"


SFDD Element::get_value() const {
    return prime.Conjoin(sub);
}


SFDD SFDD::Xor(const SFDD& sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::Conjoin(const SFDD& sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::Disjoin(const SFDD& sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}


SFDD Manager::sfddVar(int i) const {
    SFDD sfdd;
    return sfdd;
}

SFDD Manager::sfddZero() const {
    SFDD sfdd;
    return sfdd;
}

SFDD Manager::sfddOne() const {
    SFDD sfdd;
    return sfdd;
}
