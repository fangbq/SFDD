#ifndef SFDD_MANAGER
#define SFDD_MANAGER

#include "sfdd.h"
#include "sfdd_vtree.h"
#include "sfdd_nodetable.h"

namespace sfdd {

class Sfdd;

class SfddManager {
private:
    VTree vtree_;
    SfddNodeTable sfdd_node_talbe_;
public:
    SfddManager()
        : sfdd_node_talbe_() {}

    SfddManager(const VTree& vtree, const unsigned int cache_size = 1U << 16)
        : vtree_(new VTree(vtree)),
          sfdd_node_talbe_() {}

    ~SfddManager();

    // make sfdd
    Sfdd sfddZero() const;
    Sfdd sfddOne() const;
    Sfdd sfddVar(const int var) const;
    Sfdd make_sfdd_literal(const addr_t literal);
    addr_t make_sfdd_decomposition(std::vector<SfddElement>&& decomp_nodes, const int vtree_node);
    addr_t make_sfdd_literal_inner(const addr_t literal);

    // test two sfdd nodes computable or not with repect to their vtree ids
    bool computable_with(const SfddNode& lhs, const SfddNode& rhs) const;

    // Apply operations
    addr_t sfdd_apply(const OPERATOR_TYPE& op, const addr_t lhs, const addr_t rhs);
    Sfdd sfdd_intersection(const Sfdd& lhs, const Sfdd& rhs, bool do_nml = false);
    Sfdd sfdd_xor(const Sfdd& lhs, const Sfdd& rhs, bool do_nml = false);
    Sfdd sfdd_and(const Sfdd& lhs, const Sfdd& rhs, bool do_nml = false);
    Sfdd sfdd_or(const Sfdd& lhs, const Sfdd& rhs, bool do_nml = false);
   
    // normalization, compressing, trimming
    SfddNode normalization_1(const VTree & v, const SfddNode & reduced_sfdd);
    SfddNode normalization_2(const VTree & v, const SfddNode & reduced_sfdd);
    SfddNode reduced(const Sfdd& sfdd);  // reducing
    SfddNode normalized(const Sfdd& sfdd, const int lca);  // lca must be ancestor of this Sfdd!!!
    
    ZsddNode& get_zsddnode_at(const addr_t idx)  {
        return zsdd_node_table_.get_node_at(idx);
    }

};

}  // namespace sfdd


#endif
