#ifndef SFDD_H
#define SFDD_H

#include "define.h"

using namespace std;

class Vtree;
class SFDD;
class Element;
class Manager;

SFDD normalization_1(const Vtree & v, const SFDD & rsfdd, const Manager & m);
SFDD normalization_2(const Vtree & v, const SFDD & rsfdd, const Manager & m);
int get_lca(int a, int b, const Vtree & v);  // get the lowest common ancestor of two node


class Vtree {
public:
    int index = 0;  // index of vtree node
    int var = 0;  // var number
    Vtree* lt = NULL;
    Vtree* rt = NULL;
    int size = 0;
public:
    Vtree() {} ;
    Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t = TRIVIAL_TREE);
    Vtree(const Vtree & v);
    ~Vtree();
    Vtree subvtree(int index) const;  // return subvtree whose root is index
    // set<int>  get_variables() const;
    int father_of(int i) const;
    bool is_leaf(int i) const;  // return if node index is leaf
    void print(int indent = 0) const;
    void print_dot(fstream & out_dot, bool root = false) const;
    void save_file_as_dot(const string f_name) const;
};


class Manager {
public:
    Vtree* vtree = NULL;
public:
    Manager() {}
    Manager(const Vtree & v);
    ~Manager();
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
    SFDD() { elements.clear(); }
    SFDD(const SFDD & s) { value = s.value; elements = s.elements; vtree_index = s.vtree_index; }
    ~SFDD() { value = -1; elements.clear(); vtree_index = 0; }
    SFDD(int v, int i = 0) {  elements.clear(); value = v; vtree_index = i; }
    int size() const;
    inline bool is_terminal() const { /*cout << "is_terminal..." << endl;*/ return value>-1; }
    inline bool is_leaf(const Manager & m) const { return m.vtree->is_leaf(vtree_index); }  // differ with
    inline bool is_positive() const { return value>1 && value%2==0; }
    inline bool is_negative() const { return value>1 && value%2==1; }
    inline bool is_empty() const { return !is_terminal() && (elements.size()==0);}
    inline bool is_zero() const { return value==0; }
    inline bool is_one() const { return value==1; }
    inline bool is_constant() const { return is_zero() || is_one(); }
    bool computable_with(const SFDD & sfdd, const Manager & m) const;
    bool equals(const SFDD & sfdd) const;
    SFDD& operator=(const SFDD & sfdd);
    SFDD& reduced(const Manager & m);  // reducing
    SFDD& normalized(int lca, const Manager & m);  // lca must be ancestor of this SFDD!!!
    SFDD Intersection(const SFDD & s, const Manager & m, int print_info = 0) const;
    /*
     * must nml for the first time, example, x1 xor x2, if not
     * they will be calculated directly, it's not what we want
     */
    SFDD Xor(const SFDD & s, const Manager & m, bool do_nml = false) const;
    SFDD And(const SFDD & s, const Manager & m, bool do_nml = false, int clause_counter = 0) const;
    // SFDD& operator^(const SFDD & s) { return Xor(s); }
    SFDD Or(const SFDD & s, const Manager & m, bool do_nml = false) const;
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
