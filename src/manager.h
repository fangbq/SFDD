#ifndef MANAGER_H
#define MANAGER_H

#include "define.h"
#include "sfdd_node.h"
#include "sfdd_vtree.h"
#include <unordered_map>
#include <vector>
#include <stack>
#include <functional>
#include <map>
#include "readVerilog.h"
#include "cache_table.h"
#include "unique_table.h"


namespace sfdd {

class SFDD;

class Manager {
public:
    addr_t true_ = -1;
    addr_t false_ = -1;
    Vtree* vtree = NULL;
    addr_t max_addr_for_lit_ = 0;

    // for bigoplus_piterms
    std::unordered_map<int, addr_t> bigoplus_piterms;

    // fot getting lca quickly
    std::vector<std::vector<int> > lca_table_;

    UniqueTable uniq_table_;
    CacheTable cache_table_;
public:
    Manager();
    Manager(const Vtree& v, const unsigned int cache_size = 1U << 16);
    void initial_node_table_and_piterms_map();
    ~Manager();
    addr_t sfddVar(const int var);

    // size of sfdd
    unsigned long long size(const addr_t sfdd_id) const;
    unsigned long long size(const std::unordered_set<addr_t> sfdd_ids) const;

    // operations
    addr_t reduced(const SfddNode& sfdd_node);  // reducing
    addr_t generate_bigoplus_piterms(const Vtree& v);

    inline addr_t get_compl_tmn(const addr_t addr_) const { return addr_^1; }
    inline bool is_terminal(const addr_t addr_) const { return addr_>1 && addr_ <= max_addr_for_lit_; }
    inline bool is_negative(const addr_t addr_) const { return (addr_ & 1) ==1; }
    inline bool is_positive(const addr_t addr_) const { return (addr_ & 1) ==0; }
    inline bool is_zero(const addr_t addr_) const { return addr_==0; }
    inline bool is_one(const addr_t addr_) const { return addr_==1; }

    addr_t Intersection(const addr_t lhs, const addr_t rhs);

    /*
     * must nml for the first time, example, x1 xor x2, if not
     * they will be calculated directly, it's not what we want
     */
    addr_t Xor(const addr_t lhs, const addr_t rhs);

    std::vector<Element> to_partition(std::vector<Element>& alpha_);
    addr_t And(const addr_t lhs, const addr_t rhs);
    // SFDD& operator^(const SFDD& s) { return Xor(s); }
    addr_t Or(const addr_t lhs, const addr_t rhs);
    addr_t Not(const addr_t sfdd_id);


    // print node
    void print(const addr_t addr_, int indent = 0) const;
    void print(const SfddNode& sfdd_node, int indent = 0) const;

    // read cnf file
    addr_t cnf_to_sfdd(const std::string cnf_file, const std::string vtree_file = "");
    std::unordered_set<addr_t> verilog_to_sfdds(char*, const std::string vtree_file = "");
    void output_one_sfdd(logicVar *var);
};

} // namespace sfdd

#endif
