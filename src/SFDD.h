#ifndef SFDD_H
#define SFDD_H

#include "define.h"

using namespace std;

class SFDD;
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


class Manager {
public:
    Vtree* vtree;
public:
    Manager(Vtree* v) { vtree = v; };
    SFDD sfddZero();
    SFDD sfddOne();
    SFDD sfddVar(const int var);
};


class SFDD {
public:
    // int constant = 0;  // true: 1, false: -1, non-constant: 0
    // int lit = 0;  // constant:0, non-constant: x/-x
    int value = -1;  /* terminal nodes: 0 respects false, 1 respects true; 2*x respects the xth variable (x);
                        2 * x + 1 respects the negation of the xth variable (-x);
                        nonterminal: -1
                    */
    vector<Element> elements;
    int vtree_index = 0;  // respect to vtree node
public:
    SFDD() {}
    inline int size() const { return elements.size(); }
    inline bool terminal() const { return value>-1; }
    inline bool positive() const { return value>1 && value%2==0; }
    inline bool negative() const { return value>1 && value%2==1; }
    inline bool empty() const { return !terminal()&&(size()==0);}
    inline bool zero() const { return value==0; }
    inline bool one() const { return value==1; }
    bool equals(const SFDD & sfdd) const;
    SFDD& reduced(Manager & m);  // reducing
    SFDD expanded(Manager & m) const;
    SFDD Intersection(const SFDD & s, Manager & m) const;
    SFDD Xor(const SFDD & s, Manager & m) const;
    SFDD And(const SFDD & s, Manager & m) const;
    inline SFDD Or(const SFDD & s, Manager & m) const { return Xor(s, m).Xor(And(s, m), m); }
    inline SFDD Not(Manager & m) const { return Xor(m.sfddOne(), m); }
    void print(int indent = 0) const;
    void print_dot(fstream & out_dot, bool root = false, int depth = 0, string dec_name = "Dec_0_1") const;
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
    void print(int indent, int counter) const;
    void print_dot(fstream & out_dot, int depth, string e_name) const;
};



SFDD get_SFDD1(const Vtree* v, const int var);
SFDD get_SFDD2(const Vtree* v, const int rht = 0);

#endif
