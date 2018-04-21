#include "manager.h"

#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <libgen.h>
#include "sfdd.h"
#include "readVerilog.h"


extern std::map<int, int> get_index_by_var;


namespace sfdd {

Manager::Manager() : cache_table_(INIT_SIZE), sfdd_nodes_(INIT_SIZE) {
    SfddNode true_node(1), false_node(0);
    false_ = make_sfdd(false_node);
    true_ = make_sfdd(true_node);
    // std::cout << "Manager bad initialization !!!" << std::endl;
}

Manager::Manager(const Vtree& v) : cache_table_(INIT_SIZE), sfdd_nodes_(INIT_SIZE) {
    SfddNode true_node(1), false_node(0);
    false_ = make_sfdd(false_node);
    true_ = make_sfdd(true_node);
    vtree = new Vtree(v);
    initial_node_table_and_piterms_map();
};

void Manager::initial_node_table_and_piterms_map() {
    for (int i = 1; i <= (vtree->size+1)/2; ++i) {
        SfddNode pos_literal(i*2, get_index_by_var[abs(i)]), neg_literal(i*2+1, get_index_by_var[abs(i)]);
        make_sfdd(pos_literal);
        make_sfdd(neg_literal);
    }
    // create a map for vtree_node and bigoplus_piterms
    generate_bigoplus_piterms(*vtree);
    // for (auto pi : bigoplus_piterms) {
    //     std::cout << pi.first << " " << pi.second << std::endl;
    // }
}

Manager& Manager::operator=(const Manager& m_) {
    true_ = m_.true_;
    false_ = m_.false_;
    sfdd_nodes_ = m_.sfdd_nodes_;
    uniq_table_ = m_.uniq_table_;
    cache_table_ = m_.cache_table_;
    bigoplus_piterms = m_.bigoplus_piterms;
    return *this;
}

Manager::~Manager() {
    if (vtree != NULL) { delete vtree; vtree = NULL; }
};

addr_t Manager::sfddVar(const int tmp_var) {
    // in the case of the initial sfdd_nodes_[] is settel
    return (tmp_var < 0 ? (-tmp_var)*2+1 : tmp_var*2);
}

unsigned long long Manager::size(const addr_t sfdd_id) const {
    if (sfdd_nodes_[sfdd_id].is_terminal()) return 0;

    std::unordered_set<addr_t> node_ids;
    std::stack<addr_t> unexpanded;
    unexpanded.push(sfdd_id);

    while (!unexpanded.empty()) {
        addr_t e = unexpanded.top();
        unexpanded.pop();

        node_ids.insert(e);
        const SfddNode& n = sfdd_nodes_[e];
        if (n.value < 0) {
            for (const Element e : n.elements) {
                if (node_ids.find(e.first) == node_ids.end()) {
                    node_ids.insert(e.first);
                    unexpanded.push(e.first);
                }
                if (node_ids.find(e.second) == node_ids.end()) {
                    node_ids.insert(e.second);
                    unexpanded.push(e.second);
                }
            }
        }
    }

    unsigned long long int size = 0LLU;
    for (const auto& i : node_ids) {
        const SfddNode& n = sfdd_nodes_[i];
        if (n.value < 0) {
            size += n.elements.size();
        }
    }
    return size;
}

unsigned long long Manager::size(const std::unordered_set<addr_t> sfdd_ids) const {
    std::unordered_set<addr_t> node_ids;
    std::stack<addr_t> unexpanded;
    for (const auto& id : sfdd_ids) {
        if (sfdd_nodes_[id].is_terminal()) continue;
        unexpanded.push(id);
    }

    while (!unexpanded.empty()) {
        addr_t e = unexpanded.top();
        unexpanded.pop();

        node_ids.insert(e);
        const SfddNode& n = sfdd_nodes_[e];
        if (n.value < 0) {
            for (const Element e : n.elements) {
                if (node_ids.find(e.first) == node_ids.end()) {
                    node_ids.insert(e.first);
                    unexpanded.push(e.first);
                }
                if (node_ids.find(e.second) == node_ids.end()) {
                    node_ids.insert(e.second);
                    unexpanded.push(e.second);
                }
            }
        }
    }

    unsigned long long int size = 0LLU;
    for (const auto& i : node_ids) {
        const SfddNode& n = sfdd_nodes_[i];
        if (n.value < 0) {
            size += n.elements.size();
        }
    }
    return size;
}

addr_t Manager::reduced(const SfddNode& sfdd_node) {
// cout << "reduced..." << endl;
    SfddNode reduced_node = sfdd_node;
    bool isFalse = true;
    for (auto e = reduced_node.elements.begin(); \
    e != reduced_node.elements.end(); ++e) {
        if (e->second != false_) {
            isFalse = false;
            break;
        }
    }

    // 1.2 return false if all elements' subs are false
    if (isFalse)
        return false_;

    // 2 compressing
    for (auto e1 = reduced_node.elements.begin(); \
    e1 != reduced_node.elements.end(); ) {
        bool is_delete = false;
        for (auto e2 = reduced_node.elements.begin(); \
        e2 != reduced_node.elements.end(); ++e2) {
            // cout << "big equal..." << endl;
            if (e1 != e2 && e1->second==e2->second) {
                is_delete = true;
                e2->first = Xor(e1->first, e2->first);
                e1 = reduced_node.elements.erase(e1);
                break;
            }
        }
        if (!is_delete)
            ++e1;
    }

    //3 trimming
// cout << "trimming..." << endl;
    SfddNode tmp_node = reduced_node;
    if (reduced_node.elements.size() == 1) {
        // {(f, 1)} -> f
        if (reduced_node.elements[0].second == true_) {
            return tmp_node.elements[0].first;
        }
    } else if (reduced_node.elements.size() == 2) {
        // {(f, 1), (other-pi-terms\f, 0)} -> f
        // {(1, f), (other-pi-terms\1, 0)} -> f
        if (reduced_node.elements[0].second == false_) {
            if (reduced_node.elements[1].second == true_) {
                return tmp_node.elements[1].first;
            } else if (reduced_node.elements[1].first == true_) {
                return tmp_node.elements[1].second;
            }
        } else if (reduced_node.elements[1].second == false_) {
            if (reduced_node.elements[0].second == true_) {
                return tmp_node.elements[0].first;
            } else if (reduced_node.elements[0].first == true_) {
                return tmp_node.elements[0].second;
            }
        }
    }

    // if (reduced_node.is_terminal()) reduced_node.vtree_index = get_index_by_var[reduced_node.value/2];
    return make_or_find(reduced_node);
}
/*
SfddNode Manager::normalized(const addr_t sfdd_id, int lca) {
// cout << "normalized..." << endl;
    addr_t normalized_id = normalization_1(vtree->subvtree(lca), sfdd_id);
    return sfdd_nodes_[normalized_id];
}

addr_t Manager::normalization_1(const Vtree& v, const addr_t rsfdd_id) {
    SfddNode sfdd_node, rsfdd_node=sfdd_nodes_[rsfdd_id];
    sfdd_node.vtree_index = v.index;
    // base case
    if (v.var || v.index == rsfdd_node.vtree_index) {
        return rsfdd_id;
    }

    Element e1, e2;
    if (rsfdd_id == false_) {
        // rule 1.(d)
        e1.first = normalization_2(*v.lt, false_);
        e1.second = false_;
        sfdd_node.elements.push_back(e1);
        return make_or_find(sfdd_node);
    } else if (rsfdd_id == true_) {
        // rule 1.(c)
        e1.first = true_;
        e1.second = true_;
        e2.first = normalization_2(*v.lt, true_);
        e2.second = false_;
    } else if (rsfdd_node.vtree_index < v.index) {
        // rule 1.(a)
        e1.first = rsfdd_id;
        e1.second = true_;
        e2.first = normalization_2(*v.lt, rsfdd_id);
        e2.second = false_;
    } else {
        // rule 1.(b)
        e1.first = true_;
        e1.second = rsfdd_id;
        e2.first = normalization_2(*v.lt, true_);
        e2.second = false_;
    }
    if (e1.first != false_) sfdd_node.elements.push_back(e1);
    if (e2.first != false_) sfdd_node.elements.push_back(e2);
    return make_or_find(sfdd_node);  // must return a address, because each prime and sub is a address
}

/*
 * @para: v - vtree, rsfdd_node - in compiling rules 2 and 3, there
 *        are formulas like $f \oplus g$, rsfdd_node means 'g' in
 *        this kind format.
 *
addr_t Manager::normalization_2(const Vtree& v, const addr_t rsfdd_id) {
    SfddNode sfdd_node, rsfdd_node=sfdd_nodes_[rsfdd_id];
    sfdd_node.vtree_index = v.index;
    // check if constant
    if (v.var) {
        if (rsfdd_id == true_) sfdd_node.value = v.var*2;
        else if (rsfdd_id == false_) sfdd_node.value = v.var*2+1;
        else if (rsfdd_node.value%2 == 0) { sfdd_node.value = 1; sfdd_node.vtree_index = 0; }
        else return false_;
        return make_or_find(sfdd_node);
    }
    if (v.index == rsfdd_node.vtree_index)
        return Xor(normalization_2(v, false_), rsfdd_id);

    Element e1, e2;
    if (rsfdd_id == false_) {
        // rule 2.(d)
        e1.first = normalization_2(*v.lt, false_);
        e1.second = normalization_2(*v.rt, false_);
        sfdd_node.elements.push_back(e1);
        return make_or_find(sfdd_node);
    } else if (rsfdd_id == true_) {
        // rule 2.(c)
        e1.first = true_;
        e1.second = normalization_2(*v.rt, true_);
        e2.first = normalization_2(*v.lt, true_);
        e2.second = normalization_2(*v.rt, false_);
    } else if (rsfdd_node.vtree_index < v.index) {
        // rule 2.(a)
        e1.first = rsfdd_id;
        e1.second = normalization_2(*v.rt, true_);
        e2.first = normalization_2(*v.lt, rsfdd_id);
        e2.second = normalization_2(*v.rt, false_);
    } else {
        // rule 2.(b)
        e1.first = true_;
        e1.second = normalization_2(*v.rt, rsfdd_id);
        e2.first = normalization_2(*v.lt, true_);
        e2.second = normalization_2(*v.rt, false_);
    }
    if (e1.first != false_) sfdd_node.elements.push_back(e1);
    if (e2.first != false_) sfdd_node.elements.push_back(e2);
    return make_or_find(sfdd_node);
}
*/
addr_t Manager::generate_bigoplus_piterms(const Vtree& v) {
   // check if constant
   addr_t result;
   if (v.var) {
       result = v.var*2 + 1;
   } else {
       SfddNode sfdd_node;
       sfdd_node.vtree_index = v.index;
       Element e;
       e.first = generate_bigoplus_piterms(*(v.lt));
       e.second = generate_bigoplus_piterms(*v.rt);
       sfdd_node.elements.push_back(e);
       result = make_or_find(sfdd_node);
    }
    bigoplus_piterms.emplace(v.index, result);
    return result;
}

addr_t Manager::Intersection(const addr_t lhs, const addr_t rhs) {
// cout << "Intersection..." << endl;
    if (lhs > rhs) return Intersection(rhs, lhs);

    if (lhs == false_) return lhs;
    if (lhs == true_) return IntersectionOne(rhs);
    if (lhs == rhs) return lhs;

    addr_t cache = read_cache(INTER, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    SfddNode new_node;
    // normalizing for both sides
    SfddNode normalized_sfdd1 = sfdd_nodes_[lhs], normalized_sfdd2 = sfdd_nodes_[rhs];

    if (normalized_sfdd1.vtree_index == normalized_sfdd2.vtree_index) {
		new_node.vtree_index = normalized_sfdd1.vtree_index;
        if (normalized_sfdd1.is_terminal()) {
            return lhs;
            // base case
        } else {
            for (std::vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
            e1 != normalized_sfdd1.elements.end(); ++e1) {
                for (std::vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
                e2 != normalized_sfdd2.elements.end(); ++e2) {
                    Element new_e;
                    new_e.first = Intersection(e1->first, e2->first);
                    if (new_e.first != false_) {
                        new_e.second = Intersection(e1->second, e2->second);
                        new_node.elements.push_back(new_e);
                    }
                }
            }
        }
    } else {
		int lca = vtree->get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index);
		new_node.vtree_index = lca;

        if (lca != normalized_sfdd1.vtree_index && lca != normalized_sfdd2.vtree_index) {
			return (IntersectionOne(lhs) == false_ || IntersectionOne(rhs) == false_) ? false_ : true_;
        } else {
            addr_t descendant_ = rhs;
            // set normalized_sfdd1 as higher one (\alpha in paper)
            if (lca == normalized_sfdd2.vtree_index) {
                descendant_ = lhs;
                SfddNode aux_node;
                aux_node = normalized_sfdd1;
                normalized_sfdd1 = normalized_sfdd2;
                normalized_sfdd2 = aux_node;
            }
            if (normalized_sfdd2.vtree_index < normalized_sfdd1.vtree_index) {
                // normalized_sfdd2 is a left descendant of normalized_sfdd1 (1)
                for (const auto& e : normalized_sfdd1.elements) {
                    addr_t inter_ = Intersection(e.first, descendant_);
                    if (inter_ != false_) {
                        Element new_e(inter_, IntersectionOne(e.second));
                        new_node.elements.push_back(new_e);
                    }
                }
                // std::cout << "and" << std::endl;
                addr_t comp_ = Xor(bigoplus_piterms.at(vtree->subvtree(lca).lt->index), descendant_);
                if (comp_ != false_){
                    Element new_e(comp_, false_);
                    new_node.elements.push_back(new_e);
                }
            } else {
                // normalized_sfdd2 is a right descendant of normalized_sfdd1 (2)
                for (const auto& e : normalized_sfdd1.elements) {
                    addr_t inter_ = IntersectionOne(e.first);
                    if (inter_ != false_) {
                        Element new_e(inter_, Intersection(e.second, descendant_));
                        new_node.elements.push_back(new_e);
                    }
                }
                Element new_e(Xor(bigoplus_piterms.at(vtree->subvtree(lca).lt->index), true_), false_);
                new_node.elements.push_back(new_e);
            }
        }
    }
    // std::cout << "before reduce intered node ---------------" << std::endl;
    // print(new_node);
    addr_t new_id = reduced(new_node);
// std::cout << "got ---------------------" << std::endl;
// print(new_id);
    write_cache(INTER, lhs, rhs, new_id);
    return new_id;
}


addr_t Manager::IntersectionOne(const addr_t sfdd_id) {
    if (sfdd_id == true_) return true_;
    SfddNode sfdd_node = sfdd_nodes_[sfdd_id];
    if (sfdd_node.is_negative()) return true_;
    else if (sfdd_node.is_positive()) return false_;
    for (const auto& e : sfdd_node.elements)
        if (IntersectionOne(e.first) == true_)
            return IntersectionOne(e.second);
    return false_;
}

addr_t Manager::Xor(const addr_t lhs, const addr_t rhs) {
// cout << "Xor..." << endl;
    if (lhs > rhs) return Xor(rhs, lhs);

    if (lhs == false_) return rhs;
    if (lhs == true_) return XorOne(rhs);

    if (lhs == rhs) return false_;


    addr_t cache = read_cache(XOR, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    SfddNode new_node;
    SfddNode normalized_sfdd1 = sfdd_nodes_[lhs], normalized_sfdd2 = sfdd_nodes_[rhs];

    if (normalized_sfdd1.vtree_index == normalized_sfdd2.vtree_index) {
        if (normalized_sfdd1.is_terminal()) {
            return true_;
        } else {
            new_node.vtree_index = normalized_sfdd1.vtree_index;
            for (std::vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
            e1 != normalized_sfdd1.elements.end(); ++e1) {
                for (std::vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
                e2 != normalized_sfdd2.elements.end(); ++e2) {
                    Element new_e;
                    new_e.first = Intersection(e1->first, e2->first);
                    if (new_e.first != false_) {
                        new_e.second = Xor(e1->second, e2->second);
                        new_node.elements.push_back(new_e);
                    }
                }
            }
        }
    } else {
		int lca = vtree->get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index);
		new_node.vtree_index = lca;

        if (lca != normalized_sfdd1.vtree_index && lca != normalized_sfdd2.vtree_index) {
			addr_t left_descendant_, right_descendant_;
			if (normalized_sfdd1.vtree_index < lca) {
                left_descendant_ = lhs;
				right_descendant_ = rhs;
            }
			else
			{
				left_descendant_ = rhs;
				right_descendant_ = lhs;
			}

			Element e1;
			e1.first = true_;
			e1.second = (IntersectionOne(left_descendant_) == true_) ? XorOne(right_descendant_) : right_descendant_;
			new_node.elements.push_back(e1);

			Element e2;
			e2.first = Intersection(left_descendant_, Xor(bigoplus_piterms.at((vtree->subvtree(lca).lt)->index), true_));
			e2.second = true_;
			new_node.elements.push_back(e2);

            addr_t comp_ = Intersection(Xor(bigoplus_piterms.at((vtree->subvtree(lca).lt)->index), left_descendant_), \
                Xor(bigoplus_piterms.at((vtree->subvtree(lca).lt)->index), true_));
            if (comp_ != false_) {
    			Element e3(comp_, false_);
    			new_node.elements.push_back(e3);
            }



        } else {
            addr_t descendant_ = rhs;
            // set normalized_sfdd1 as higher one (\alpha in paper)
            if (lca == normalized_sfdd2.vtree_index) {
                descendant_ = lhs;
                SfddNode aux_node;
                aux_node = normalized_sfdd1;
                normalized_sfdd1 = normalized_sfdd2;
                normalized_sfdd2 = aux_node;
            }

            if (normalized_sfdd2.vtree_index < normalized_sfdd1.vtree_index) {
                // normalized_sfdd2 is a left descendant of normalized_sfdd1 (1)
				addr_t comp_beta = Xor(bigoplus_piterms.at(vtree->subvtree(lca).lt->index), descendant_);

                for (const auto& e : normalized_sfdd1.elements) {
                    addr_t inter_ = Intersection(e.first, descendant_);
                    if (inter_ != false_) {
                        Element new_e(inter_, XorOne(e.second));
                        new_node.elements.push_back(new_e);
                    }

					inter_ = Intersection(e.first, comp_beta);
                    if (inter_ != false_) {
                        Element new_e(inter_, e.second);
                        new_node.elements.push_back(new_e);
                    }
                }

            } else {
                // normalized_sfdd2 is a right descendant of normalized_sfdd1 (2)
                for (const auto& e : normalized_sfdd1.elements) {
                    addr_t inter_ = IntersectionOne(e.first);
                    if (inter_ != false_) {
                        Element new_e(inter_, Xor(e.second, descendant_));
                        new_node.elements.push_back(new_e);
						break;
                    }
                }

				addr_t comp_true = Xor(bigoplus_piterms.at(vtree->subvtree(lca).lt->index), true_);
				for (const auto& e : normalized_sfdd1.elements) {
                    addr_t inter_ = Intersection(e.first, comp_true);
                    if (inter_ != false_) {
                        Element new_e(inter_, e.second);
                        new_node.elements.push_back(new_e);
                    }
                }
            }

        }
    }

    // std::cout << "before reduce xored node ---------------" << std::endl;
    // print(new_node);
    addr_t new_id = reduced(new_node);
    write_cache(XOR, lhs, rhs, new_id);
    return new_id;
}

addr_t Manager::XorOne(const addr_t sfdd_id) {
    if (sfdd_id == false_) return true_;
    if (sfdd_id == true_) return false_;
    SfddNode sfdd_node = sfdd_nodes_[sfdd_id], new_node;
    new_node.vtree_index = sfdd_node.vtree_index;
    if (sfdd_node.is_terminal()) {
        return sfdd_node.value^1;  // a trick
    }
    for (const auto& e : sfdd_node.elements) {
        if (IntersectionOne(e.first) == true_) {
            Element new_e1(true_, XorOne(e.second));
            new_node.elements.push_back(new_e1);
            addr_t prime_ = XorOne(e.first);
            if (prime_ != false_) {
                Element new_e2(prime_, e.second);
                new_node.elements.push_back(new_e2);
            }
        } else {
            new_node.elements.push_back(e);
        }
    }
    // std::cout << "before reduce xoroneed node ---------------" << std::endl;
    // print(new_node);
    return reduced(new_node);
}
std::vector<Element> Manager::to_partition(std::vector<Element>& alpha_) {
    std::vector<Element> beta_, prime_combined_;

    sort(alpha_.begin(), alpha_.end());
    // combine elements with same prime
    for (const auto& e : alpha_) {
        if (!prime_combined_.empty() && prime_combined_.back().first==e.first) {
            Element new_e(e.first, Xor(prime_combined_.back().second, e.second));
            prime_combined_.back() = new_e;
        } else {
            prime_combined_.push_back(e);
        }
    }

    for (const auto& ps_ : prime_combined_) {
        std::vector<Element> gamma_;
        addr_t o_ = ps_.first;
        Element inter_e;
        for (const auto& qr_ : beta_) {
            inter_e.first = Intersection(qr_.first, o_);
            if (inter_e.first != false_) {
                inter_e.second = Xor(qr_.second, ps_.second);
                gamma_.push_back(inter_e);  // add inter-ele
            }
            Element origin_e;
            origin_e.first = Xor(qr_.first, inter_e.first);
            if (origin_e.first != false_) {
                origin_e.second = qr_.second;
                gamma_.push_back(origin_e);  // add orig-ele
            }
            o_ = Xor(o_, inter_e.first);
        }
        if (o_ != false_) {
            Element last_(o_, ps_.second);
            gamma_.push_back(last_);
        }
        beta_ = gamma_;
    }
    return beta_;
}

addr_t Manager::And(const addr_t lhs, const addr_t rhs) {
// cout << "And..." << endl;
    if (lhs > rhs) return And(rhs, lhs);

    if (lhs == false_) return lhs;
    if (lhs == true_) return rhs;

    if (lhs == rhs) return lhs;

    addr_t cache = read_cache(AND, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    SfddNode new_node;
    SfddNode normalized_sfdd1 = sfdd_nodes_[lhs], normalized_sfdd2 = sfdd_nodes_[rhs];
    if (normalized_sfdd1.vtree_index == normalized_sfdd2.vtree_index) {
        if (normalized_sfdd1.is_terminal()) {
            return false_;
        }
        new_node.vtree_index = normalized_sfdd1.vtree_index;

        // get pre decompositions
        std::vector<Element> alpha_;
        for (const auto& e1 : normalized_sfdd1.elements) {
            for (const auto& e2 : normalized_sfdd2.elements) {
                addr_t prime_product = And(e1.first, e2.first);
                if (prime_product != false_) {
                    addr_t sub_product = And(e1.second, e2.second);
                    Element new_e(prime_product, sub_product);
                    alpha_.push_back(new_e);
                }
            }
        }
        // ToPartition(\alpha)
        new_node.elements = to_partition(alpha_);
    } else {
        int lca = vtree->get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index);
        addr_t full_piterms = bigoplus_piterms.at(vtree->subvtree(lca).lt->index);
        new_node.vtree_index = lca;

        if (lca != normalized_sfdd1.vtree_index && lca != normalized_sfdd2.vtree_index) {
            addr_t left_descendant_, right_descendant_;
			if (normalized_sfdd1.vtree_index < lca) {
                left_descendant_ = lhs;
				right_descendant_ = rhs;
            }
			else
			{
				left_descendant_ = rhs;
				right_descendant_ = lhs;
			}

			Element e1(left_descendant_, right_descendant_);
			new_node.elements.push_back(e1);

            if (left_descendant_ != full_piterms) {
            	Element e2(Xor(left_descendant_, full_piterms), false_);
    			new_node.elements.push_back(e2);
            }
        } else {
            addr_t descendant_ = rhs;
            // set normalized_sfdd1 as higher one (\alpha in paper)
            if (lca == normalized_sfdd2.vtree_index) {
                descendant_ = lhs;
                SfddNode aux_node;
                aux_node = normalized_sfdd1;
                normalized_sfdd1 = normalized_sfdd2;
                normalized_sfdd2 = aux_node;
            }
            if (normalized_sfdd2.vtree_index < normalized_sfdd1.vtree_index) {
                std::vector<Element> decomps;
                for (const auto& e : normalized_sfdd1.elements) {
                    addr_t prime_ = And(e.first, descendant_);
                    if (prime_ != false_) {
                        Element new_e(prime_, e.second);
                        decomps.push_back(new_e);
                    }
                }
                if (descendant_ != full_piterms) {
                    Element e(And(Xor(descendant_, full_piterms), full_piterms), false_);
        			decomps.push_back(e);
                }
                // ToPartition
                new_node.elements = to_partition(decomps);
            } else {
                for (const auto& e : normalized_sfdd1.elements) {
                    Element new_e(e.first, And(e.second, descendant_));
                    new_node.elements.push_back(new_e);
                }
            }
        }
    }
    // std::cout << "before reduce anded node ---------------" << std::endl;
    // print(new_node);
    addr_t new_id = reduced(new_node);
    write_cache(AND, lhs, rhs, new_id);
    return new_id;
}

addr_t Manager::Or(const addr_t lhs, const addr_t rhs) {
// cout << "Or..." << endl;
    if (lhs > rhs) return Or(rhs, lhs);

    if (lhs == false_) return rhs;
    if (lhs == true_) return true_;
    if (lhs == rhs) return lhs;

    addr_t cache = read_cache(OR, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    // addr_t new_id = Xor(Xor(lhs, rhs), And(lhs, rhs));
    addr_t new_id = Not(And(Not(lhs), Not(rhs)));  // method 2, seems faster

    write_cache(OR, lhs, rhs, new_id);
    return new_id;
}

addr_t Manager::Not(const addr_t sfdd_id) {
    return Xor(true_, sfdd_id);
}

addr_t Manager::make_sfdd(const SfddNode& new_node) {
    sfdd_nodes_.emplace_back();
    size_t node_id = sfdd_nodes_.size()-1;
    uniq_table_.emplace(new_node, node_id);
    sfdd_nodes_[node_id] = new_node;
    return node_id;
}

addr_t Manager::make_or_find(const SfddNode& new_node) {
    SfddNode sorted_node = new_node;
    sort(sorted_node.elements.begin(), sorted_node.elements.end());
    auto res = uniq_table_.find(sorted_node);
    if (res != uniq_table_.end()) {
        return res->second;
    }
    return make_sfdd(sorted_node);
}

void Manager::print_sfdd_nodes() const {
    std::cout << "sfdd_nodes_:-------------------------------" << std::endl;
    int i = 0;
    for (auto& sfdd_node: sfdd_nodes_) {
        std::cout << "Node " << i++ << ":" << std::endl;
        print(sfdd_node);
        std::cout << std::endl;
    }
}

void Manager::print_unique_table() const {
    std::cout << "unique_table:-------------------------------" << std::endl;
    for (auto& x: uniq_table_) {
        std::cout << "Node addr_t: " << x.second << std::endl;
        print(x.first);
        std::cout << std::endl;
    }
}

void Manager::write_cache(const OPERATOR_TYPE op, const addr_t lhs,
                 const addr_t rhs, const addr_t res) {
    auto key = calc_key(op, lhs, rhs);
    cache_table_[key] = std::make_tuple(op, lhs, rhs, res);
}

void Manager::clear_cache() {
    for (auto it =  cache_table_.begin(); it != cache_table_.end(); ++it) {
        *it = std::make_tuple(OPERATOR_TYPE::NULLOP, -1, -1, -1);
    }
}

addr_t Manager::read_cache(const OPERATOR_TYPE op, const addr_t lhs, const addr_t rhs) {
    auto key = calc_key(op, lhs, rhs);
    auto res = cache_table_[key];

    if (std::get<0>(res) == op &&
        std::get<1>(res) == lhs &&
        std::get<2>(res) == rhs) {
        // std::cout << "got one: " << op << " " << lhs << " " << rhs << " " << get<3>(res) << endl;
        return std::get<3>(res);
    }
    return SFDD_NULL;
}

size_t Manager::calc_key(const OPERATOR_TYPE op, const addr_t lhs,  const addr_t rhs) {
    size_t key = 0;
    hash_combine(key, std::hash<int>()(static_cast<int>(op)));
    hash_combine(key, std::hash<size_t>()(lhs));
    hash_combine(key, std::hash<size_t>()(rhs));
    // std::cout << "cache table size: " << cache_table_.size() << std::endl;
    return key % cache_table_.size();
}

void Manager::print_cache_table() const {
    std::cout << "cache_table:-------------------------------" << std::endl;
    for (auto& x: cache_table_) {
        std::cout << std::get<0>(x) << std::get<1>(x) << std::get<2>(x) << std::get<3>(x) << std::endl;
    }
}

void Manager::print(const addr_t addr_, int indent) const {
    SfddNode sfdd_node = sfdd_nodes_[addr_];
    if (sfdd_node.elements.empty()) {
        for (int i = 0; i < indent; ++i) std::cout << " ";
        if (sfdd_node.value < 2) {
            // std::cout << sfdd_node.value << std::endl;
            std::cout << sfdd_node.value << " ~ " << sfdd_node.vtree_index << std::endl;
        } else {
            if (sfdd_node.is_negative()) std::cout << "-";
            // std::cout << "x" << sfdd_node.value/2 << std::endl;
            std::cout << "x" << sfdd_node.value/2 << " ~ " << sfdd_node.vtree_index << std::endl;
        }
        return;
    }
    for (int i = 0; i < indent; ++i) std::cout << " ";
    std::cout << "Dec " << sfdd_node.vtree_index << ":" << std::endl;
    int counter = 1;
    indent++;
    for (const auto e : sfdd_node.elements) {
        for (int i = 0; i < indent; ++i) std::cout << " ";
        std::cout << "E" << counter << "p:" << std::endl;
        print(e.first, indent+1);
        for (int i = 0; i < indent; ++i) std::cout << " ";
        std::cout << "E" << counter++ << "s:" << std::endl;
        print(e.second, indent+1);
    }
    return;
}

void Manager::print(const SfddNode& sfdd_node, int indent) const {
    if (sfdd_node.elements.empty()) {
        for (int i = 0; i < indent; ++i) std::cout << " ";
        if (sfdd_node.value < 2) {
            // std::cout << sfdd_node.value << std::endl;
            std::cout << sfdd_node.value << " ~ " << sfdd_node.vtree_index << std::endl;
        } else {
            if (sfdd_node.is_negative()) std::cout << "-";
            // std::cout << "x" << sfdd_node.value/2 << std::endl;
            std::cout << "x" << sfdd_node.value/2 << " ~ " << sfdd_node.vtree_index << std::endl;
        }
        return;
    }
    for (int i = 0; i < indent; ++i) std::cout << " ";
    std::cout << "Dec " << sfdd_node.vtree_index << ":" << std::endl;
    int counter = 1;
    indent++;
    for (const auto& e : sfdd_node.elements) {
        for (int i = 0; i < indent; ++i) std::cout << " ";
        std::cout << "E" << counter << "p:" << std::endl;
        print(e.first, indent+1);
        for (int i = 0; i < indent; ++i) std::cout << " ";
        std::cout << "E" << counter++ << "s:" << std::endl;
        print(e.second, indent+1);
    }
    return;
}

addr_t Manager::cnf_to_sfdd(const std::string cnf_file, const std::string vtree_file) {
    // read *.cnf
    std::ifstream infile(cnf_file, std::ios::in);
    std::string file_name = static_cast<std::string>(cnf_file);
    std::cout << file_name.substr(file_name.find_last_of("/")+1, file_name.length()-file_name.find_last_of("/")) << "\t";
    if(!infile)
    {
        std::cerr << "open infile error!" << std::endl;
        exit(1);
    }
    int var_no = 0, col_no = 0;  // Number of variables and number of clauses
    std::string line;
    while (!infile.eof()) {
        std::getline(infile, line);

        if (line.length() == 0 || line[0] == 'c')
            ; //cout << "IGNORE LINE\n";
        else {
            var_no = stoi(line.substr(6, line.find_last_of(' ')-6));  //  p cnf a b; this line can't end with ' '
            col_no = stoi(line.substr(line.find_last_of(' ')+1, line.length()-line.find_last_of(' ')));
            break;
        }
    }
    std::cout << var_no << "  " << col_no << "    ";
    if (vtree_file.empty()) {
        std::vector<int> vars_order;
        for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);
        vtree = new Vtree(1, var_no*2-1, vars_order);
    } else {
        vtree = new Vtree(vtree_file);
    }
    initial_node_table_and_piterms_map();

    // v.save_vtree_file("s27_ balanced.vtree");

    // make sfdd literal by literal
    addr_t fml = true_;
    int  clause_counter = 1;
    for(int line = 0; line < col_no; ++line)  //read every line number, and save as a clause
    {
        addr_t clause = false_;
        while (true) {
            int var;
            infile >> var;
            if (var == 0) break;
            addr_t tmp_var = sfddVar(var);
            clause = Or(clause, tmp_var);
        }
        fml = And(fml, clause);
        std::cout << "clause : " << clause_counter++ << " done; " << size(fml) << std::endl;
    }
    return fml;
}

std::unordered_set<addr_t> Manager::verilog_to_sfdds(char* cnf_file, const std::string vtree_file) {
    logicNet *net = readVerilog(cnf_file);

    // std::cout << "readVerilog done;" << std::endl;

    if (vtree_file.empty()) {
        std::vector<int> vars_order;
        for (unsigned int i = 1; i <= net->Nin; ++i) vars_order.push_back(i);
        vtree = new Vtree(1, net->Nin*2-1, vars_order);
    } else {
        vtree = new Vtree(vtree_file);
    }
    initial_node_table_and_piterms_map();
    // std::cout << "readVerilog done;" << std::endl;

    std::unordered_set<addr_t> ids;
    // int  output_counter = 1;
    for(unsigned int i = 0; i < net->Nout; ++i){
        output_one_sfdd(&(net->output[i]));
        addr_t node_id = (addr_t)((net->output[i]).func);
        // std::cout << "output " << output_counter++ << " done, size: " << size(node_id) << std::endl;
        ids.emplace(node_id);
    }
    // print_sfdd_nodes();
    std::string test_name(basename(cnf_file));
    std::cout << test_name << "\t\t" << net->Nin << "\t" << net->Nout << "\t" << net->Nwire << "\t";
    freeLogicNet(net);
    return ids;
}

void Manager::output_one_sfdd(logicVar *var) {
    // build SFDD literal
    if(VARTYPE(var->info) == _INPUT_){
        var->func = sfddVar(var->varIndex);
        return;
    }
    // build SFDD formulae
    unsigned int gateType = GATETYPE(var->info);
    if(gateType == _XOR_ || gateType == _AND_ || gateType == _OR_){
        output_one_sfdd(var->A);  output_one_sfdd(var->B);
        addr_t funcA = (addr_t)(var->A->func);
        addr_t funcB = (addr_t)(var->B->func);
        if(gateType == _XOR_)       { var->func = Xor(funcA, funcB); }
        else if(gateType == _AND_)  { var->func = And(funcA, funcB); }
        else                { var->func = Or(funcA, funcB); }
    }else{  // INV or BUF
        output_one_sfdd(var->A);
        addr_t funcA = (addr_t)(var->A->func);

        if(gateType == _INV_)       { var->func = Not(funcA); }
        else                { var->func = funcA; }
    }
    return;
}

} // namespace sfdd
