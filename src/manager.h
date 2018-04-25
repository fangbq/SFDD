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


namespace sfdd {

class SFDD;

class Manager {
public:
    addr_t true_ = -1;
    addr_t false_ = -1;
    Vtree* vtree = NULL;
    addr_t max_addr_for_lit_ = 0;
    // for unique table
    std::unordered_map<SfddNode, addr_t> uniq_table_;
    std::vector<SfddNode> sfdd_nodes_;

    // for cache table
    const unsigned int INIT_SIZE = 1U<<10;
    std::vector<cache_entry> cache_table_;

    // for bigoplus_piterms
    std::unordered_map<int, addr_t> bigoplus_piterms;

    // fot getting lca quickly
    std::vector<std::vector<int> > lca_table_;
public:
    Manager();
    Manager(const Vtree& v);
    void initial_node_table_and_piterms_map();
    Manager& operator=(const Manager& m_);
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

    // for unique table
    addr_t make_sfdd(const SfddNode& new_node);
    addr_t make_or_find(const SfddNode& new_node);  // to avoid storing redundant sfdds
    void print_sfdd_nodes() const;
    void print_unique_table() const;

    // for cache table
    void write_cache(const OPERATOR_TYPE op, const addr_t lhs,
                     const addr_t rhs, const addr_t res);
    void clear_cache();
    addr_t read_cache(const OPERATOR_TYPE op, const addr_t lhs, const addr_t rhs);
    size_t calc_key(const OPERATOR_TYPE op, const addr_t lhs,  const addr_t rhs);
    void print_cache_table() const;

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
