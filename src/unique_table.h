#ifndef UNQUE_TABLE_H_
#define UNQUE_TABLE_H_
#include "sfdd_node.h"
#include <vector>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <algorithm>


namespace sfdd {

class UniqueTable {
public:

    UniqueTable() :
        sfdd_nodes_(),
        uniq_table_() {}

    SfddNode& get_node_at(const addr_t i) {
        auto& n = sfdd_nodes_[i];
        return n;
    }
    const SfddNode& get_node_at(const addr_t i) const {
        auto& n = sfdd_nodes_[i];
        return n;
    }

	addr_t make_sfdd(const SfddNode& new_node) {
	    // sfdd_nodes_.emplace_back();
	    size_t node_id = sfdd_nodes_.size();
	    uniq_table_.emplace(new_node, node_id);
	    sfdd_nodes_.push_back(new_node);
	    return node_id;
	}

	addr_t make_or_find(const SfddNode& new_node) {
	    SfddNode sorted_node = new_node;
	    sort(sorted_node.elements.begin(), sorted_node.elements.end());
	    auto res = uniq_table_.find(sorted_node);
	    if (res != uniq_table_.end()) {
	        return res->second;
	    }
	    return make_sfdd(sorted_node);
	}

public:
    std::vector<SfddNode> sfdd_nodes_;
    std::unordered_map<SfddNode, addr_t> uniq_table_;
};

}

#endif // ENDIF