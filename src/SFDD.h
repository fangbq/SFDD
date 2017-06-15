#ifndef SFDD_H
#define SFDD_H

#include "define.h"

using namespace std;

class Element;
class Manager;

class Vtree {
public:
    int index = 0;  // index of vtree node
    int var = 0;  // var number
    Vtree* lt = NULL;
    Vtree* rt = NULL;
    int size = 0;
public:
    Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t = TRIVIAL_TREE);
    set<int>  get_variables() const;
    void print(int indent = 0) const;
};


class SFDD {
public:
    int constant = 0;  // true: 1, false: -1, non-constant: 0
    int lit = 0;  // constant:0, non-constant: x/-x
    vector<Element> elements;
    int vtree_index = 0;  // respect to vtree node
public:
    SFDD() {}
    inline int size() const { return elements.size(); }
    inline bool terminal() const { return constant+lit; }
    inline bool empty() const { return !terminal()&&(size()==0);}
    inline bool zero() const { return constant==-1; }
    inline bool one() const { return constant==1; }
    bool equals(const SFDD & sfdd) const;
    SFDD& reduced(Manager & m);  // reducing
    SFDD expanded(Manager & m) const;
    SFDD Intersection(const SFDD & s, Manager & m) const;
    SFDD Xor(const SFDD & s, Manager & m) const;
    SFDD And(const SFDD & s, Manager & m) const;
    SFDD Or(const SFDD & s, Manager & m) const;
    void print(int indent = 0) const;
};


class Element {
public:
    SFDD prime;
    SFDD sub;
public:
    Element() {}
    Element(const Element& e) { prime = e.prime; sub = e.sub; }
    inline bool equals(const Element & e) const {
        return prime.equals(e.prime) && sub.equals(e.sub);
    };
};


class Manager {
public:
    Vtree* vtree;
public:
    Manager(Vtree* v) { vtree = v; };
    SFDD sfddZero() const;
    SFDD sfddOne() const;
    SFDD sfddVar(const Vtree* v, const int var) const;
    SFDD get_SFDD(const Vtree* v, const int rht = -1) const;
};


#endif
