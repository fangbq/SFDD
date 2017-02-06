#ifndef SFDD_H
#define SFDD_H

#include <vector>
#include "define.h"

using namespace std;

class Element;

class SFDD {
private:
    vector<Element> elements;
    size_t size;
public:
    SFDD() { size = 0; }
    SFDD(const SFDD& s) { elements = s.get_elements(); size = s.get_size(); }
    size_t get_size() const { return size; }
    vector<Element> get_elements() const { return elements; }
    SFDD Xor(const SFDD& s) const;
    SFDD Conjoin(const SFDD& s) const;
    SFDD Disjoin(const SFDD& s) const;
};


class Element {
private:
    SFDD prime;
    SFDD sub;
public:
    Element() {}
    Element(const Element& e) { prime = e.prime; sub = e.sub; }
    SFDD get_prime() const { return prime; }
    SFDD get_sub() const { return sub; }
    SFDD get_value() const;
};


class Manager {
public:
    SFDD sfddVar(int i) const;
    SFDD sfddZero() const;
    SFDD sfddOne() const;
};


#endif
