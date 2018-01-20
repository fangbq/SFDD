#ifndef SFDD_H
#define SFDD_H

#include "define.h"

using namespace std;

class Vtree;
class SFDD;
class Element;
class Manager;
class SfddNodeTable;

SFDD normalization_1(const Vtree & v, const SFDD& rsfdd, Manager & m);
SFDD normalization_2(const Vtree & v, const SFDD& rsfdd, Manager & m);
int get_lca(int a, int b, const Vtree & v);  // get the lowest common ancestor of two node


class Vtree {
public:
    int index = 0;  // index of vtree node
    int var = 0;  // var number
    Vtree* lt = NULL;
    Vtree* rt = NULL;
    int size = 0;
public:
    Vtree() {}
    Vtree(const int index);
    Vtree(const int index, const int var);
    Vtree(const int parent, const int left_child, const int right_child);
    Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t = TRIVIAL_TREE);
    Vtree(const Vtree& v);
    Vtree(const string& file_name);  // import from file
    Vtree& merge(const Vtree& hat);
    ~Vtree();
    Vtree subvtree(int index) const;  // return subvtree whose root is index
    // set<int>  get_variables() const;
    int father_of(int i) const;
    bool is_leaf(int i) const;  // return if node index is leaf
    void print(int indent = 0) const;
    void print_dot(fstream& out_dot, bool root = false) const;
    void save_dot_file(const string f_name) const;
    void print_vtree(fstream& out_dot, bool root = false) const;
    void save_vtree_file(const string f_name) const;
};


class SFDD {
public:
    // int constant = 0;  // true: 1, false: -1, non-constant: 0
    // int lit = 0;  // constant:0, non-constant: x/-x
    int value = -1;  /* terminal nodes: 0 respects false, 1 respects true; 2*x respects the xth variable (x);
                        2*x+1 respects the negation of the xth variable (-x);
                        nonterminal: -1
                    */
    vector<Element> elements;
    int vtree_index = 0;  // respect to vtree node
    int hash_value = 0;
public:
    SFDD() { elements.clear(); }
    SFDD(const SFDD& s) { value = s.value; elements = s.elements; vtree_index = s.vtree_index; }
    ~SFDD() { value = -1; elements.clear(); vtree_index = 0; }
    SFDD(int v, int i = 0) { elements.clear(); value = v; vtree_index = i; }
    int size() const;
    inline bool is_terminal() const { return value!=-1; }
    inline bool is_positive() const { return value>1 && value%2==0; }
    inline bool is_negative() const { return value>1 && value%2==1; }
    inline bool is_zero() const { return value==0; }
    inline bool is_one() const { return value==1; }
    inline bool is_constant() const { return is_zero() || is_one(); }
    bool computable_with(const SFDD& sfdd, const Manager& m) const;
    bool operator==(const SFDD& sfdd) const;
    SFDD& operator=(const SFDD& sfdd);
    SFDD& reduced(Manager& m);  // reducing
    SFDD& normalized(int lca, Manager& m);  // lca must be ancestor of this SFDD!!!
    SFDD Intersection(const SFDD& s, Manager & m) const;
    /*
     * must nml for the first time, example, x1 xor x2, if not
     * they will be calculated directly, it's not what we want
     */
    SFDD Xor(const SFDD& s, Manager & m) const;
    SFDD And(const SFDD& s, Manager & m) const;
    // SFDD& operator^(const SFDD& s) { return Xor(s); }
    SFDD Or(const SFDD& s, Manager & m) const;
    inline SFDD Not(Manager& m) const;
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
    inline bool equals(const Element& e) const {
        return prime==e.prime && sub==e.sub;
    };
    void print(int indent, int counter) const;
    void print_dot(fstream& out_dot, int depth, string e_name) const;
};

namespace std {
template <> struct hash<SFDD> {
    std::size_t operator()(const SFDD& n) const {
        size_t h = 0;
        if (n.value < 0) {
            hash_combine(h, hash<int>()(n.value));
            hash_combine(h, hash<int>()(n.vtree_index));
            return h;
        } else if (n.value >= 0) {
            for (const auto& e : n.elements) {
                hash_combine(h, hash<SFDD>()(e.prime));
                hash_combine(h, hash<SFDD>()(e.sub));
            }
            hash_combine(h, hash<int>()(n.vtree_index));
        } 
        return h;
    }
};
}

class Manager {
public:
    Vtree* vtree = NULL;
    // for unique table
    vector<SFDD> sfdd_nodes_;
    unordered_map<SFDD, addr_t> uniq_table_;

    // for cache table
    const unsigned int INIT_SIZE = 1U<<8;
    vector<cache_entry> cache_table_;
public:
    Manager() : cache_table_(INIT_SIZE) {}
    Manager(const Vtree& v);
    ~Manager();
    SFDD sfddZero();
    SFDD sfddOne();
    SFDD sfddVar(const int var);

    // for unique table
    addr_t make_sfdd(const SFDD& sfdd);
    addr_t make_or_find(const SFDD& sfdd);  // to avoid storing redundant sfdds
    void print_sfdd_nodes() const;
    void print_unique_table() const;

    // for cache table
    void write_cache(const OPERATOR_TYPE op, const addr_t lhs, 
                     const addr_t rhs, const addr_t res);
    void clear_cache();
    addr_t read_cache(const OPERATOR_TYPE op, const addr_t lhs, const addr_t rhs);
    size_t calc_key(const OPERATOR_TYPE op, const addr_t lhs,  const addr_t rhs);
    void print_cache_table() const;
};


#endif
