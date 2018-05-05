#include "manager.h"

#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <libgen.h>
#include <chrono>
#include "sfdd.h"
#include "readVerilog.h"


extern std::map<int, int> get_index_by_var;

using namespace std::chrono;

namespace sfdd {

Manager::Manager() {
    SfddNode true_node(1), false_node(0);
    false_ = uniq_table_.make_sfdd(false_node);
    true_ = uniq_table_.make_sfdd(true_node);
    // std::cout << "Manager bad initialization !!!" << std::endl;
}

Manager::Manager(const Vtree& v, const unsigned int cache_size) :
    cache_table_(cache_size) {
    SfddNode true_node(1), false_node(0);
    false_ = uniq_table_.make_sfdd(false_node);
    true_ = uniq_table_.make_sfdd(true_node);
    vtree = new Vtree(v);
    initial_node_table_and_piterms_map();
};

void Manager::initial_node_table_and_piterms_map() {
    for (int i = 1; i <= (vtree->size+1)/2; ++i) {
        SfddNode pos_literal(i*2, get_index_by_var[abs(i)]), neg_literal(i*2+1, get_index_by_var[abs(i)]);
        uniq_table_.make_sfdd(pos_literal);
        uniq_table_.make_sfdd(neg_literal);
    }
    max_addr_for_lit_ = vtree->size+2;
    // create a map for vtree_node and bigoplus_piterms
    generate_bigoplus_piterms(*vtree);

    // initial lca_table_
    std::vector<int> v;
    lca_table_.push_back(v);
    for (int i = 1; i <= vtree->size; ++i) {
        std::vector<int> table;
        table.push_back(0);
        for (int j = 1; j <= vtree->size; ++j) {
            table.push_back(vtree->get_lca(i, j));
        }
        lca_table_.push_back(table);
    }
}

Manager::~Manager() {
    if (vtree != NULL) { delete vtree; vtree = NULL; }
};

addr_t Manager::sfddVar(const int tmp_var) {
    // in the case of the initial sfdd_nodes_[] is settel
    return (tmp_var < 0 ? (-tmp_var)*2+1 : tmp_var*2);
}

unsigned long long Manager::size(const addr_t sfdd_id) const {
    if (is_terminal(sfdd_id)) return 0;

    std::unordered_set<addr_t> node_ids;
    std::stack<addr_t> unexpanded;
    unexpanded.push(sfdd_id);

    while (!unexpanded.empty()) {
        addr_t e = unexpanded.top();
        unexpanded.pop();

        node_ids.insert(e);
        const SfddNode& n = uniq_table_.get_node_at(e);
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
        const SfddNode& n = uniq_table_.get_node_at(i);
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
        if (is_terminal(id)) continue;
        unexpanded.push(id);
    }

    while (!unexpanded.empty()) {
        addr_t e = unexpanded.top();
        unexpanded.pop();

        node_ids.insert(e);
        const SfddNode& n = uniq_table_.get_node_at(e);
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
        const SfddNode& n = uniq_table_.get_node_at(i);
        if (n.value < 0) {
            size += n.elements.size();
        }
    }
    return size;
}

addr_t Manager::reduced(const SfddNode& sfdd_node) {
// cout << "reduced..." << endl;
    bool sameSub = true;
    addr_t first_sub = sfdd_node.elements.front().second;
    for (const auto& e : sfdd_node.elements) {
        if (e.second != first_sub)
        {
            sameSub = false;
            break;
        }
    }

    SfddNode result_node;
    // 1.2 return false if all elements' subs are false
    if (sameSub) {
        if (first_sub == false_) return false_;
        else if (first_sub == true_) return bigoplus_piterms.at(vtree->subvtree(sfdd_node.vtree_index).lt->index);
        else {
            Element e(bigoplus_piterms.at(vtree->subvtree(sfdd_node.vtree_index).lt->index), first_sub);
            result_node.vtree_index = sfdd_node.vtree_index;
            result_node.elements.push_back(e);
            return uniq_table_.make_or_find(result_node);
        }
    }

    // 2 compressing
    result_node = sfdd_node;
    for (auto e1 = result_node.elements.begin(); \
    e1 != result_node.elements.end(); ) {
        bool is_delete = false;
        for (auto e2 = result_node.elements.begin(); \
        e2 != result_node.elements.end(); ++e2) {
            // cout << "big equal..." << endl;
            if (e1 != e2 && e1->second==e2->second) {
                is_delete = true;
                e2->first = Xor(e1->first, e2->first);
                e1 = result_node.elements.erase(e1);
                break;
            }
        }
        if (!is_delete)
            ++e1;
    }

    //3 trimming
// cout << "trimming..." << endl;
    SfddNode tmp_node = result_node;
    if (result_node.elements.size() == 2) {
        // {(f, 1), (other-pi-terms\f, 0)} -> f
        // {(1, f), (other-pi-terms\1, 0)} -> f
        if (result_node.elements[0].second == false_) {
            if (result_node.elements[1].second == true_) {
                return tmp_node.elements[1].first;
            } else if (result_node.elements[1].first == true_) {
                return tmp_node.elements[1].second;
            }
        } else if (result_node.elements[1].second == false_) {
            if (result_node.elements[0].second == true_) {
                return tmp_node.elements[0].first;
            } else if (result_node.elements[0].first == true_) {
                return tmp_node.elements[0].second;
            }
        }
    }

    // if (result_node.is_terminal()) result_node.vtree_index = get_index_by_var[result_node.value/2];
    return uniq_table_.make_or_find(result_node);
}

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
       result = uniq_table_.make_sfdd(sfdd_node);
    }
    bigoplus_piterms.emplace(v.index, result);
    return result;
}

addr_t Manager::Intersection(const addr_t lhs, const addr_t rhs) {
// cout << "Intersection..." << endl;
    if (lhs > rhs) return Intersection(rhs, lhs);
    if (lhs == rhs || lhs == false_) return lhs;

    if (is_terminal(lhs) && is_terminal(rhs))
    {
        if (get_compl_tmn(lhs) == rhs)
        {
            return lhs;
        }
        return (is_negative(lhs) && is_negative(rhs)) ? true_ : false_;
    }

    addr_t cache = cache_table_.read_cache(INTER, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    if (lhs == true_) {
        if (is_terminal(rhs))
        {
            return is_negative(rhs) ? true_ : false_;
        }
        addr_t result_ = false_;
        SfddNode sfdd_node = uniq_table_.get_node_at(rhs);
        for (const auto& e : sfdd_node.elements)
            if (Intersection(true_, e.first) == true_) {
                result_ = Intersection(true_, e.second);
                break;
            }
        cache_table_.write_cache(INTER, true_, rhs, result_);
        return result_;
    }




    SfddNode new_node;
    // normalizing for both sides
    SfddNode normalized_sfdd1 = uniq_table_.get_node_at(lhs), normalized_sfdd2 = uniq_table_.get_node_at(rhs);

    if (normalized_sfdd1.vtree_index == normalized_sfdd2.vtree_index) {
        if (is_terminal(lhs)) {
            return lhs;
        }
        new_node.vtree_index = normalized_sfdd1.vtree_index;
        if (normalized_sfdd1.elements.size() == 1) {
            for (std::vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
            e2 != normalized_sfdd2.elements.end(); ++e2) {
                Element new_e(e2->first, Intersection(normalized_sfdd1.elements[0].second, e2->second));
                new_node.elements.push_back(new_e);
            }
        } else if (normalized_sfdd2.elements.size() == 1) {
            for (std::vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
            e1 != normalized_sfdd1.elements.end(); ++e1) {
                Element new_e(e1->first, Intersection(normalized_sfdd2.elements[0].second, e1->second));
                new_node.elements.push_back(new_e);
            }
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
        int lca = lca_table_[normalized_sfdd1.vtree_index][normalized_sfdd2.vtree_index];
		new_node.vtree_index = lca;

        if (lca != normalized_sfdd1.vtree_index && lca != normalized_sfdd2.vtree_index) {
			return (Intersection(true_, lhs) == false_ || Intersection(true_, rhs) == false_) ? false_ : true_;
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
                // INT (1)
                addr_t bigoplus_ = bigoplus_piterms.at(vtree->subvtree(lca).lt->index);
                if (bigoplus_ == descendant_) {
                    for (const auto& e : normalized_sfdd1.elements) {
                        Element new_e(e.first, Intersection(true_, e.second));
                        new_node.elements.push_back(new_e);
                    }
                } else {
                    // normalized_sfdd2 is a left descendant of normalized_sfdd1 (1)
                    for (const auto& e : normalized_sfdd1.elements) {
                        addr_t inter_ = Intersection(e.first, descendant_);
                        if (inter_ != false_) {
                            Element new_e(inter_, Intersection(true_, e.second));
                            new_node.elements.push_back(new_e);
                        }
                    }
                    Element new_e(Xor(bigoplus_, descendant_), false_);
                    new_node.elements.push_back(new_e);
                }
            } else {
                // normalized_sfdd2 is a right descendant of normalized_sfdd1 (2)
                for (const auto& e : normalized_sfdd1.elements) {
                    addr_t inter_ = Intersection(true_, e.first);
                    if (inter_ != false_) {
                        Element new_e(inter_, Intersection(e.second, descendant_));
                        new_node.elements.push_back(new_e);
                    }
                }
                Element new_e(Xor(true_, bigoplus_piterms.at(vtree->subvtree(lca).lt->index)), false_);
                new_node.elements.push_back(new_e);
            }
        }
    }
    // std::cout << "before reduce intered node ---------------" << std::endl;
    // print(new_node);
    addr_t new_id = reduced(new_node);
// std::cout << "got ---------------------" << std::endl;
// print(new_id);
    cache_table_.write_cache(INTER, lhs, rhs, new_id);
    return new_id;
}

addr_t Manager::Xor(const addr_t lhs, const addr_t rhs) {
// cout << "Xor..." << endl;
    if (lhs > rhs) return Xor(rhs, lhs);

    if (lhs == false_) return rhs;
    if (lhs == rhs) return false_;


    addr_t cache = cache_table_.read_cache(XOR, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    if (lhs == true_) {
        // Xor One
        if (is_terminal(rhs))
            return get_compl_tmn(rhs);

        SfddNode sfdd_node = uniq_table_.get_node_at(rhs), new_node;
        new_node.vtree_index = sfdd_node.vtree_index;
        auto it_ = sfdd_node.elements.begin();
        for ( ; it_ != sfdd_node.elements.end(); ++it_) {
            if (Intersection(true_, it_->first) == true_) {
                Element new_e1(true_, Xor(true_, it_->second));
                new_node.elements.push_back(new_e1);
                addr_t prime_ = Xor(true_, it_->first);
                if (prime_ != false_) {
                    Element new_e2(prime_, it_->second);
                    new_node.elements.push_back(new_e2);
                    break;
                }
            } else {
                new_node.elements.push_back(*it_);
            }
        }
        if (it_ != sfdd_node.elements.end())
            new_node.elements.insert(new_node.elements.end(), ++it_, sfdd_node.elements.end());

        addr_t new_id = reduced(new_node);
        cache_table_.write_cache(XOR, lhs, rhs, new_id);
        return new_id;
    }


    SfddNode new_node;
    SfddNode normalized_sfdd1 = uniq_table_.get_node_at(lhs), normalized_sfdd2 = uniq_table_.get_node_at(rhs);

    if (normalized_sfdd1.vtree_index == normalized_sfdd2.vtree_index) {
        // when respect same vtree node
        if (is_terminal(lhs)) {
            return true_;
        }
        new_node.vtree_index = normalized_sfdd1.vtree_index;
        if (normalized_sfdd1.elements.size() == 1) {
            for (std::vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
            e2 != normalized_sfdd2.elements.end(); ++e2) {
                Element new_e(e2->first, Xor(normalized_sfdd1.elements[0].second, e2->second));
                new_node.elements.push_back(new_e);
            }
        } else if (normalized_sfdd2.elements.size() == 1) {
            for (std::vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
            e1 != normalized_sfdd1.elements.end(); ++e1) {
                Element new_e(e1->first, Xor(normalized_sfdd2.elements[0].second, e1->second));
                new_node.elements.push_back(new_e);
            }
        } else {
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
        int lca = lca_table_[normalized_sfdd1.vtree_index][normalized_sfdd2.vtree_index];
		new_node.vtree_index = lca;

        if (lca != normalized_sfdd1.vtree_index && lca != normalized_sfdd2.vtree_index) {
            // Xor (3)
			addr_t left_descendant_, right_descendant_;
			if (normalized_sfdd1.vtree_index < lca) {
                left_descendant_ = lhs;
				right_descendant_ = rhs;
            } else {
				left_descendant_ = rhs;
				right_descendant_ = lhs;
			}

			Element e1;
			e1.first = true_;
			e1.second = (Intersection(true_, left_descendant_) == true_) ? Xor(true_, right_descendant_) : right_descendant_;
			new_node.elements.push_back(e1);

			Element e2;
			e2.first = Intersection(left_descendant_, Xor(true_, bigoplus_piterms.at((vtree->subvtree(lca).lt)->index)));
			e2.second = true_;
			new_node.elements.push_back(e2);

            addr_t comp_ = Intersection(Xor(bigoplus_piterms.at((vtree->subvtree(lca).lt)->index), left_descendant_), \
                Xor(true_, bigoplus_piterms.at((vtree->subvtree(lca).lt)->index)));
            if (comp_ != false_) {
    			Element e3(comp_, false_);
    			new_node.elements.push_back(e3);
            }

        } else {
            // Xor (1), (2)
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
                addr_t bigoplus_ = bigoplus_piterms.at(vtree->subvtree(lca).lt->index);
                if (bigoplus_ == descendant_) {
                    for (const auto& e : normalized_sfdd1.elements) {
                        Element new_e(e.first, Xor(true_, e.second));
                        new_node.elements.push_back(new_e);
                    }
                } else {
    				addr_t comp_beta = Xor(bigoplus_, descendant_);

                    for (const auto& e : normalized_sfdd1.elements) {
                        addr_t inter_ = Intersection(e.first, descendant_);
                        if (inter_ != false_) {
                            Element new_e(inter_, Xor(true_, e.second));
                            new_node.elements.push_back(new_e);
                        }

    					inter_ = Intersection(e.first, comp_beta);
                        if (inter_ != false_) {
                            Element new_e(inter_, e.second);
                            new_node.elements.push_back(new_e);
                        }
                    }
                }

            } else {
                // normalized_sfdd2 is a right descendant of normalized_sfdd1 (2)
                auto it_ = normalized_sfdd1.elements.begin();
                for ( ; it_ != normalized_sfdd1.elements.end(); ++it_) {
                    if (Intersection(true_, it_->first) == true_) {
                        Element new_e1(true_, Xor(descendant_, it_->second));
                        new_node.elements.push_back(new_e1);
                        addr_t prime_ = Xor(true_, it_->first);
                        if (prime_ != false_) {
                            Element new_e2(prime_, it_->second);
                            new_node.elements.push_back(new_e2);
                            break;
                        }
                    } else {
                        new_node.elements.push_back(*it_);
                    }
                }
                if (it_ != normalized_sfdd1.elements.end())
                    new_node.elements.insert(new_node.elements.end(), ++it_, normalized_sfdd1.elements.end());
            }

        }
    }

    addr_t new_id = reduced(new_node);
    cache_table_.write_cache(XOR, lhs, rhs, new_id);
    return new_id;
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
        auto it_ = beta_.begin();
        for ( ; it_ != beta_.end(); ++it_) {
            inter_e.first = Intersection(it_->first, o_);
            if (inter_e.first != false_) {
                inter_e.second = Xor(it_->second, ps_.second);
                gamma_.push_back(inter_e);  // add inter-ele
            }
            Element origin_e;
            origin_e.first = Xor(it_->first, inter_e.first);
            if (origin_e.first != false_) {
                origin_e.second = it_->second;
                gamma_.push_back(origin_e);  // add orig-ele
            }
            o_ = Xor(o_, inter_e.first);
            if (o_ == false_) {
                break;
            }
        }
        if (it_ != beta_.end())
            gamma_.insert(gamma_.end(), ++it_, beta_.end());

        if (o_ != false_) {
            Element last_(o_, ps_.second);
            gamma_.push_back(last_);
        }
        beta_ = gamma_;
    }
    return beta_;
}

addr_t Manager::And(const addr_t lhs, const addr_t rhs) {
    // return Xor(lhs, rhs);
// cout << "And..." << endl;
    if (lhs > rhs) return And(rhs, lhs);

    if (lhs == false_) return lhs;
    if (lhs == true_) return rhs;

    if (lhs == rhs) return lhs;

    addr_t cache = cache_table_.read_cache(AND, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    SfddNode new_node;
    SfddNode normalized_sfdd1 = uniq_table_.get_node_at(lhs), normalized_sfdd2 = uniq_table_.get_node_at(rhs);
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
        int lca = lca_table_[normalized_sfdd1.vtree_index][normalized_sfdd2.vtree_index];
        addr_t full_piterms = bigoplus_piterms.at(vtree->subvtree(lca).lt->index);
        new_node.vtree_index = lca;

        if (lca != normalized_sfdd1.vtree_index && lca != normalized_sfdd2.vtree_index) {
            addr_t left_descendant_, right_descendant_;
			if (normalized_sfdd1.vtree_index < lca) {
                left_descendant_ = lhs;
				right_descendant_ = rhs;
            } else {
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

    addr_t new_id = reduced(new_node);
    cache_table_.write_cache(AND, lhs, rhs, new_id);
    return new_id;
}

addr_t Manager::Or(const addr_t lhs, const addr_t rhs) {
// cout << "Or..." << endl;
    if (lhs > rhs) return Or(rhs, lhs);

    if (lhs == false_) return rhs;
    if (lhs == true_) return true_;
    if (lhs == rhs) return lhs;

    addr_t cache = cache_table_.read_cache(OR, lhs, rhs);
    if (cache != SFDD_NULL)
        return cache;

    // addr_t new_id = Xor(Xor(lhs, rhs), And(lhs, rhs));
    addr_t new_id = Not(And(Not(lhs), Not(rhs)));  // method 2, seems faster

    cache_table_.write_cache(OR, lhs, rhs, new_id);
    return new_id;
}

addr_t Manager::Not(const addr_t sfdd_id) {
    return Xor(true_, sfdd_id);
}

void Manager::print(const addr_t addr_, int indent) const {
    SfddNode sfdd_node = uniq_table_.get_node_at(addr_);
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
    std::ifstream vtreefile(vtree_file, std::ios::in);
    if (!vtreefile || vtree_file.empty()) {
        std::vector<int> vars_order;
        for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);
        vtree = new Vtree(1, var_no*2-1, vars_order, RANDOM_TREE);
    } else {
        vtree = new Vtree(vtree_file);
    }
    initial_node_table_and_piterms_map();

    // v.save_vtree_file("s27_ balanced.vtree");

    // make sfdd literal by literal
    addr_t fml = true_;
    int  clause_counter = 1;
    std::vector<addr_t> clause_sfdds;
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
        clause_sfdds.push_back(clause);
        std::cout << "clause : " << clause_counter++ << " done; " << size(fml) << std::endl;
    }
    return fml;
    while (clause_sfdds.size() > 1) {
        std::vector<addr_t> tmp_sfdds;
        for (int i = 0; i < (int)(clause_sfdds.size() + 1) / 2; i++) {
            if (2 * i + 1 >= (int)clause_sfdds.size()) {
                tmp_sfdds.push_back(clause_sfdds[2*i]);
            }
            else {
                tmp_sfdds.push_back(And(clause_sfdds[2*i], clause_sfdds[2*i+1]));
            }
        }
        clause_sfdds = tmp_sfdds;
    }
    return clause_sfdds[0];
}

std::unordered_set<addr_t> Manager::verilog_to_sfdds(char* cnf_file, const std::string vtree_file) {
    logicNet *net = readVerilog(cnf_file);
    std::string test_name(basename(cnf_file));
    // std::cout << "readVerilog done;" << std::endl;

    std::ifstream vtreefile(vtree_file, std::ios::in);
    if (!vtreefile || vtree_file.empty()) {
        std::vector<int> vars_order;
        for (unsigned int i = 1; i <= net->Nin; ++i) vars_order.push_back(i);

        // for random vtree
        std::chrono::milliseconds ms = duration_cast< std::chrono::milliseconds >(
            system_clock::now().time_since_epoch()
        );
        srand (unsigned (ms.count() ) );
        random_shuffle(vars_order.begin(), vars_order.end(), myrandom);

        vtree = new Vtree(1, net->Nin*2-1, vars_order, RANDOM_TREE);
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
