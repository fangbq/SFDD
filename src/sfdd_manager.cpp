#include "sfdd_manager.h"

namespace sfdd {


Manager::~Manager() {
    if (vtree != NULL)
    	delete vtree;
};

Sfdd SfddManager::sfddZero() const {
    return make_sfdd_literal(0); 
}

Sfdd SfddManager::sfddOne() const {
    return make_sfdd_literal(1);
}

Sfdd SfddManager::sfddVar(const int tmp_var) {
    return make_sfdd_literal(tmp_var);
}

addr_t SfddManager::make_sfdd_decomposition(std::vector<SfddElement>&& decomp_nodes, 
                                            const int vtree_node) {
    assert(!decomp_nodes.empty());
    return sfdd_node_table_.make_or_find_decomp(std::move(decomp_nodes), vtree_node);
}

Sfdd SfddManager::make_sfdd_literal(const addr_t literal) {
    addr_t res = make_sfdd_literal_inner(literal);
    return Sfdd(res, *this);
}

addr_t SfddManager::make_sfdd_literal_inner(const addr_t literal) {
    const int v_id = vtree_.find_literal_node_id(literal);
    return sfdd_node_table_.make_or_find_literal(literal, v_id);
}

bool SfddManager::computable_with(const SfddNode& lhs, const SfddNode& rhs) const {
    return is_terminal() && sfdd.is_terminal() && (is_constant() || sfdd.is_constant() || \
        vtree_index == sfdd.vtree_index);
}

addr_t SfddManager::sfdd_apply(const OPERATOR_TYPE& op, const addr_t lhs_addr, const addr_t rhs_addr) {
	SfddNode lhs = get_sfddnode_at(lhs_addr), rhs = get_sfddnode_at(rhs_addr);

	// trival case
	if (op == OPERATOR_TYPE::INTERSECTION) {
	    if (lhs.is_empty() || rhs.is_empty()) cout << "big error" << endl;
	    if (lhs.is_zero()) return lhs_addr;
	    if (rhs.is_zero()) return rhs_addr;
	} else if (op == OPERATOR_TYPE::XOR) {
	    if (lhs.is_empty()) return rhs_addr;
	    if (rhs.is_empty()) return lhs_addr;
	    if (lhs.is_zero()) return rhs_addr;
	    if (rhs.is_zero()) return lhs_addr;
	} else if (op == OPERATOR_TYPE::AND) {
	    if (lhs.is_empty()) return rhs_addr;
	    if (rhs.is_empty()) return lhs_addr;
	    if (lhs.is_zero()) return lhs_addr;
	    if (rhs.is_zero()) return rhs_addr;
	    if (lhs.is_one()) return rhs_addr;
	    if (rhs.is_one()) return lhs_addr;
	} else {
		std::cerr << "sfdd_apply: OPERATOR_TYPE error" << std::endl;
	}

    SfddNode new_sfdd_node;
    new_sfdd_node.vtree_index = vtree_index;
    // normalizing for both sides
    SfddNode normalized_sfdd1 = lhs, normalized_sfdd2 = rhs;
    if (!computable_with(lhs, rhs) && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, *vtree);
        normalized_sfdd1.normalized(lca);
        normalized_sfdd2.normalized(lca);
        new_sfdd_node.vtree_index = lca;
    }

    // nontrival case
	if (op == OPERATOR_TYPE::INTERSECTION) {
	    if (normalized_sfdd1.is_terminal() && normalized_sfdd2.is_terminal()) {
	        // base case
	        if (normalized_sfdd1==normalized_sfdd2 || normalized_sfdd2.is_negative())
	            new_sfdd = normalized_sfdd1;
	        else if (normalized_sfdd1.is_negative())
	            new_sfdd = normalized_sfdd2;
	        else
	            new_sfdd_node.value = 0;
	    } else {
	        for (vector<SfddElement>::const_iterator e1 = normalized_sfdd1.elements.begin();
	        e1 != normalized_sfdd1.elements.end(); ++e1) {
	            for (vector<SfddElement>::const_iterator e2 = normalized_sfdd2.elements.begin();
	            e2 != normalized_sfdd2.elements.end(); ++e2) {
	                SfddElement new_e;
	                new_e.prime = Intersection(e1->prime, e2->prime);

	                if (!new_e.prime.is_zero()) {
	                    new_e.sub = Intersection(e1->sub, e2->sub);
	                    new_sfdd_node.elements.push_back(new_e);
	                }
	            }
	        }
	    }
	} else if (op == OPERATOR_TYPE::XOR) {
	    if (normalized_sfdd1.is_terminal() && normalized_sfdd2.is_terminal()) {
	        // base case
	        if (normalized_sfdd1==normalized_sfdd2) {
	            new_sfdd_node.value = 0;
	        } else if (normalized_sfdd1.is_zero() || normalized_sfdd2.is_zero()) {
	            new_sfdd_node.value = normalized_sfdd1.value+normalized_sfdd2.value;
	        } else if (normalized_sfdd1.is_one()) {
	            new_sfdd_node.value = normalized_sfdd2.value/2*2+1-normalized_sfdd2.value%2;
	        } else if (normalized_sfdd2.is_one()) {
	            new_sfdd_node.value = normalized_sfdd1.value/2*2+1-normalized_sfdd1.value%2;
	        } else {
	            new_sfdd_node.value = 1;
	        }
	    } else {
	        for (vector<SfddElement>::const_iterator e1 = normalized_sfdd1.elements.begin();
	        e1 != normalized_sfdd1.elements.end(); ++e1) {
	            for (vector<SfddElement>::const_iterator e2 = normalized_sfdd2.elements.begin();
	            e2 != normalized_sfdd2.elements.end(); ++e2) {
	                SfddElement new_e;
	                new_e.prime = .Intersection(e1->prime, e2->prime, do_nml);

	                if (!new_e.prime.is_zero()) {
	                    new_e.sub = Xor(e1->sub, e2->sub);
	                    new_sfdd_node.elements.push_back(new_e);
	                }
	            }
	        }
	    }
	} else if (op == OPERATOR_TYPE::AND) {
	    if (normalized_sfdd1.is_terminal() && normalized_sfdd2.is_terminal()) {
	        // base case
	        if (normalized_sfdd1.is_zero() || normalized_sfdd2.is_zero()) {
	            new_sfdd_node.value = 0;
	        } else if (normalized_sfdd1.is_one() || normalized_sfdd2.is_one()) {
	            new_sfdd_node.value = normalized_sfdd1.value*normalized_sfdd2.value;
	        } else if (normalized_sfdd1==normalized_sfdd2) {
	            new_sfdd = normalized_sfdd1;
	        } else {
	            new_sfdd_node.value = 0;
	        }
	    } else {
	        for (vector<SfddElement>::const_iterator e1 = normalized_sfdd1.elements.begin();
	        e1 != normalized_sfdd1.elements.end(); ++e1) {
	            for (vector<SfddElement>::const_iterator e2 = normalized_sfdd2.elements.begin();
	            e2 != normalized_sfdd2.elements.end(); ++e2) {
	                Sfdd prime_product = And(e1->prime, e2->prime);
	                Sfdd sub_product = And(e1->sub, e2->sub);
	                Sfdd tmp_new_sfdd_node;
	                tmp_new_sfdd_node.vtree_index = new_sfdd_node.vtree_index;
	                for (vector<SfddElement>::const_iterator new_e = new_sfdd_node.elements.begin();
	                new_e != new_sfdd_node.elements.end(); ++new_e) {
	                    SfddElement inter_e;
	                    inter_e.prime = Intersection(new_e->prime, prime_product);
	                    if (!inter_e.prime.is_zero()) {
	                        SfddElement origin_e;
	                        inter_e.sub = Xor(new_e->sub, sub_product);
	                        tmp_new_sfdd_node.elements.push_back(inter_e);  // add inter-ele
	                        origin_e.prime = Xor(new_e->prime, inter_e.prime);
	                        origin_e.sub = new_e->sub;
	                        tmp_new_sfdd_node.elements.push_back(origin_e);  // add orig-ele
	                        prime_product = Xor(prime_product, inter_e.prime);
	                    } else {
	                        tmp_new_sfdd_node.elements.push_back(*new_e);
	                    }
	                }
	                if (!prime_product.is_zero()) {
	                    SfddElement last_e;  // last element that has removed all common parts with others
	                    last_e.prime = prime_product;
	                    last_e.sub = sub_product;
	                    tmp_new_sfdd_node.elements.push_back(last_e);  // add last element
	                }
	                new_sfdd = tmp_new_sfdd_node;
	            }
	        }
	    }
	} else {
		std::cerr << "sfdd_apply: OPERATOR_TYPE error" << std::endl;
	}

    new_sfdd_node.reduced(m);

    if (new_sfdd_node.is_terminal()) new_sfdd_node.vtree_index = get_index_by_var[new_sfdd_node.value/2];
    return new_sfdd_node;
}

Sfdd SfddManager::sfdd_intersection(const Sfdd& lhs, const Sfdd& rhs, bool do_nml) {
    addr_t res = sfdd_apply(OPERATOR_TYPE::INTERSECTION, lhs.addr(), rhs.addr());
    return Sfdd(res, *this);
}

Sfdd SfddManager::sfdd_xor(const Sfdd& lhs, const Sfdd& rhs, bool do_nml) {
    addr_t res = sfdd_apply(OPERATOR_TYPE::XOR, lhs.addr(), rhs.addr());
    return Sfdd(res, *this);
}

Sfdd SfddManager::sfdd_and(const Sfdd& lhs, const Sfdd& rhs, bool do_nml) {
	addr_t res = sfdd_apply(OPERATOR_TYPE::AND, lhs.addr(), rhs.addr());
    return Sfdd(res, *this);
}

Sfdd SfddManager::sfdd_or(const Sfdd& lhs, const Sfdd& rhs, bool do_nml) {
	addr_t res = sfdd_apply(OPERATOR_TYPE::OR, lhs.addr(), rhs.addr());
    return Sfdd(res, *this);
}


SfddNode SfddNode::normalization_1(const Vtree & v, const SfddNode & rsfdd_node) {
    SfddNode sfdd_node;
    sfdd_node.vtree_index = v.index;
    // base case
    if (v.var || (!rsfdd_node.is_terminal() && v.index == rsfdd_node.vtree_index)) {
        sfdd_node = rsfdd_node_node;
        sfdd_node.vtree_index = v.index;
        return sfdd_node;
    }

    SfddElement e1, e2;
    if (rsfdd_node.is_zero()) {
        // rule 1.(d)
        e1.prime = normalization_2(*v.lt, sfddZero());
        e1.sub = normalization_1(*v.rt, sfddZero());
        sfdd_node.sfdd_node.decomp_.push_back(e1);
        return sfdd_node;
    } else if (rsfdd_node.is_one()) {
        // rule 1.(c)
        e1.prime = normalization_1(*v.lt, sfddOne());
        e1.sub = normalization_1(*v.rt, sfddOne());  // normalization
        e2.prime = normalization_2(*v.lt, sfddOne());
        e2.sub = normalization_1(*v.rt, sfddZero());
    } else if (rsfdd_node.vtree_index < v.index) {
        // rule 1.(a)
        e1.prime = normalization_1(*v.lt, rsfdd_node);  // normalization
        e1.sub = normalization_1(*v.rt, sfddOne());
        e2.prime = normalization_2(*v.lt, rsfdd_node);
        e2.sub = normalization_1(*v.rt, sfddZero());
    } else {
        // rule 1.(b)
        e1.prime = normalization_1(*v.lt, sfddOne());
        e1.sub = normalization_1(*v.rt, rsfdd_node);  // normalization
        e2.prime = normalization_2(*v.lt, sfddOne());
        e2.sub = normalization_1(*v.rt, sfddZero());
    }
    if (!e1.prime.is_zero()) sfdd_node.sfdd_node.decomp_.push_back(e1);
    if (!e2.prime.is_zero()) sfdd_node.sfdd_node.decomp_.push_back(e2);
    return sfdd_node;
}

/*
 * @para: v - vtree, rsfdd_node - in compiling rules 2 and 3, there
 *        are formulas like $f \oplus g$, rsfdd_node means 'g' in
 *        this kind format.
 */
SfddNode SfddNode::normalization_2(const Vtree & v, const Sfdd & rsfdd_node) {
    SfddNode sfdd_node;
    sfdd_node.vtree_index = v.index;
    // check if constant
    if (v.var) {
        if (rsfdd_node.is_one()) sfdd_node.literal_ = v.var*2;
        else if (rsfdd_node.is_zero()) sfdd_node.literal_ = v.var*2+1;
        else if (rsfdd_node.literal_%2 == 0) sfdd_node.literal_ = 1;
        else sfdd_node.literal_ = 0;
        return sfdd_node;
    }
    if (v.index == rsfdd_node.vtree_index)
        return normalization_2(v, sfddZero()).Xor(rsfdd_node);

    SfddElement e1, e2;
    if (rsfdd_node.is_zero()) {
        // rule 2.(d)
        e1.prime = normalization_2(*v.lt, sfddZero());
        e1.sub = normalization_2(*v.rt, sfddZero());
        sfdd_node.sfdd_node.decomp_.push_back(e1);
        return sfdd_node;
    } else if (rsfdd_node.is_one()) {
        // rule 2.(c)
        e1.prime = normalization_1(*v.lt, sfddOne());
        e1.sub = normalization_2(*v.rt, sfddOne());  // normalization
        e2.prime = normalization_2(*v.lt, sfddOne());
        e2.sub = normalization_2(*v.rt, sfddZero());
    } else if (rsfdd_node.vtree_index < v.index) {
        // rule 2.(a)
        e1.prime = normalization_1(*v.lt, rsfdd_node);  // normalization
        e1.sub = normalization_2(*v.rt, sfddOne());
        e2.prime = normalization_2(*v.lt, rsfdd_node);
        e2.sub = normalization_2(*v.rt, sfddZero());
    } else {
        // rule 2.(b)
        e1.prime = normalization_1(*v.lt, sfddOne());
        e1.sub = normalization_2(*v.rt, rsfdd_node);  // normalization
        e2.prime = normalization_2(*v.lt, sfddOne());
        e2.sub = normalization_2(*v.rt, sfddZero());
    }
    if (!e1.prime.is_zero()) sfdd_node.sfdd_node.decomp_.push_back(e1);
    if (!e2.prime.is_zero()) sfdd_node.sfdd_node.decomp_.push_back(e2);
    return sfdd_node;
}

Sfdd SfddManager::reduced(const Sfdd& sfdd) {
// cout << "reduced..." << endl;
	SfddNode sfdd_node = get_sfddnode_at(sfdd.addr());

    if (sfdd_node.is_terminal()) return sfdd;
    bool valid = false;
    for (vector<SfddElement>::iterator e = sfdd_node.decomp_.begin(); \
    e != sfdd_node.decomp_.end(); ) {
        // 1.1 removes those decomp_ that primes are false
        if (reduced(e->prime).is_zero()) {
            e = sfdd_node.decomp_.erase(e);
        } else if (!reduced(e->sub).is_zero()) {
            valid = true;
            ++e;
        } else {
            ++e;
        }
    }
    // 1.2 return false if all decomp_' subs are false
    if (!valid) sfdd_node.decomp_.clear();
    if (size()==0) literal_ = 0;
    //3 trimming
    if (sfdd_node.decomp_.size() == 1) {
        // {(-a, T)} -> -a
        if (sfdd_node.decomp_[0].prime.is_terminal() && sfdd_node.decomp_[0].sub.is_one()) {
            literal_ = sfdd_node.decomp_[0].prime.literal_;
            sfdd_node.decomp_.clear();
        }
    }
    if (sfdd_node.decomp_.size() == 2) {
        // {(a, 1), (other-pi-terms/a, 0)} -> a
        // {(1, a), (other-pi-terms/1, 0)} -> a
        if (sfdd_node.decomp_[0].sub.is_zero()) {
            if (sfdd_node.decomp_[1].sub.is_one() && sfdd_node.decomp_[1].prime.is_terminal()) {
                literal_ = sfdd_node.decomp_[1].prime.literal_;
                sfdd_node.decomp_.clear();
            } else if (sfdd_node.decomp_[1].prime.is_one() && sfdd_node.decomp_[1].sub.is_terminal()) {
                literal_ = sfdd_node.decomp_[1].sub.literal_;
                sfdd_node.decomp_.clear();
            }
        } else if (sfdd_node.decomp_[1].sub.is_zero()) {
            if (sfdd_node.decomp_[0].sub.is_one() && sfdd_node.decomp_[0].prime.is_terminal()) {
                literal_ = sfdd_node.decomp_[0].prime.literal_;
                sfdd_node.decomp_.clear();
            } else if (sfdd_node.decomp_[0].prime.is_one() && sfdd_node.decomp_[0].sub.is_terminal()) {
                literal_ = sfdd_node.decomp_[0].sub.literal_;
                sfdd_node.decomp_.clear();
            }
        }
    }
    // 2 compressing (necessary)
    for (vector<SfddElement>::iterator e1 = sfdd_node.decomp_.begin(); \
    e1 != sfdd_node.decomp_.end(); ) {
        bool is_delete = false;
        for (vector<SfddElement>::iterator e2 = sfdd_node.decomp_.begin(); \
        e2 != sfdd_node.decomp_.end(); ++e2) {
            // cout << "big equal..." << endl;
            if (e1 != e2 && e1->sub==e2->sub) {
                is_delete = true;
                e2->prime = e2->prime.Xor(e1->prime).reduced(m);
                e1 = sfdd_node.decomp_.erase(e1);
                break;
            }
        }
        if (!is_delete)
            ++e1;
    }

    if (is_terminal()) vtree_index = get_index_by_var[literal_/2];
    return *this;
}

int SfddManager::get_lca(int a, int b, const Vtree & v) {
    if (v.index == a || v.index == b) return v.index;
    int L = v.lt ? get_lca(a, b, *v.lt) : 0;
    int R = v.rt ? get_lca(a, b, *v.rt) : 0;
    if (L && R) return v.index;  // if p and q are on both sides
    return L ? L : R;  // either one of p,q is on one side OR p,q is not in L&R subtrees
}

Sfdd SfddManager::normalized(const Sfdd& sfdd, const int lca) {
	SfddNode sfdd_node = get_sfddnode_at(sfdd.addr());
	
    // normalize \alpha from vtree_index to bottom
    for (vector<SfddElement>::iterator e = sfdd_node.decomp_.begin(); \
    e != sfdd_node.decomp_.end(); ++e) {
        e->prime.normalized(vtree->subvtree(vtree_index).lt->index);
        e->sub.normalized(vtree->subvtree(vtree_index).rt->index);
    }
    *this = normalization_1(vtree->subvtree(lca), *this);

    if (!is_terminal()) literal_ = -1;
    return *this;
}


}  // namespace sfdd
