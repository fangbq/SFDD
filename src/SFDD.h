#ifndef SFDD_H
#define SFDD_H

#include "define.h"

using namespace std;

class Vtree;
class SFDD;
class Element;
class Manager;

SFDD normalization_1(const Vtree* v, const SFDD & rsfdd, const Manager & m);
SFDD normalization_2(const Vtree* v, const SFDD & rsfdd, const Manager & m);
int get_lca(int a, int b, Vtree * v);  // get the lowest common ancestor of two node


class Vtree {
public:
    int index = 0;  // index of vtree node
    int var = 0;  // var number
    Vtree* lt = NULL;
    Vtree* rt = NULL;
    int size = 0;
public:
    Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t = TRIVIAL_TREE);
    Vtree(const Vtree & v);
    Vtree* subvtree(int index);  // return subvtree whose root is index
    // set<int>  get_variables() const;
    bool leaf(int index);  // return if node index is leaf
    void print(int indent = 0) const;
    void print_dot(fstream & out_dot, bool root = false) const;
    void save_file_as_dot(const string f_name) const;
};


class Manager {
public:
    Vtree* vtree;
public:
    Manager(Vtree* v) { vtree = v; };
    SFDD sfddZero() const;
    SFDD sfddOne() const;
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
    SFDD(int v, int i = 0) { value = v; vtree_index = i; }
    int size() const;
    inline bool terminal() const { return value>-1; }
    inline bool positive() const { return value>1 && value%2==0; }
    inline bool negative() const { return value>1 && value%2==1; }
    inline bool empty() const { return !terminal() && (elements.size()==0);}
    inline bool zero() const { return value==0; }
    inline bool one() const { return value==1; }
    bool computable_with(const SFDD & sfdd, const Manager & m) const;
    bool equals(const SFDD & sfdd) const;
    SFDD& reduced(const Manager & m);  // reducing
    SFDD& normalized(int lca, const Manager & m);  // lca must be ancestor of this SFDD!!!
    SFDD Intersection(const SFDD & s, const Manager & m) const;
    SFDD Xor(const SFDD & s, const Manager & m) const;
    SFDD And(const SFDD & s, const Manager & m) const;
    // SFDD& operator^(const SFDD & s) { return Xor(s); }
    SFDD Or(const SFDD & s, const Manager & m) const;
    inline SFDD Not(const Manager & m) const { return Xor(m.sfddOne(), m); }
    void print(int indent = 0) const;
    void print_dot(fstream & out_dot, bool root = false, int depth = 0, string dec_name = "Dec_0_1") const;
    void save_file_as_dot(const string f_name) const;
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


#endif
