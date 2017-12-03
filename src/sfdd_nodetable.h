#ifndef SFDD_NODE_TABLE
#define SFDD_NODE_TABLE

#include "sfdd.h"
#include "sfdd_node.h"

namespace sfdd {

class Sfdd;

class SfddNodeTable {
public:
	SfddNodeTable() :
		sfdd_nodes_(),
		uniq_table_(),
		avail_() {}

    SfddNode& get_node_at(const addr_t i) {
        auto& n = sfdd_nodes_[i];
        return n;
    }
    // why need two??
    const SfddNode& get_node_at(const addr_t i) const {
        auto& n = sfdd_nodes_[i];
        return n;
    }

    addr_t make_or_find_literal(const addr_t literal, const int v_id) {
    	SfddNode new_node(literal, v_id);
        return make_or_find_inner(std::move(new_node));
    }

    addr_t make_or_find_decomp(std::vector<SfddElement>&& decomp, const int v_id) {
        std::sort(decomp.begin(), decomp.end());
        SfddNode new_node(std::move(decomp), v_id);
        return make_or_find_inner(std::move(new_node));
    }

    addr_t make_or_find_inner(SfddNode&& new_node) {
        auto res = uniq_table_.find(new_node);
        if (res != uniq_table_.end()) {
            return res->second;
        }

        size_t node_id = new_node_id();
        uniq_table_.emplace(new_node, node_id);
        // sfdd_nodes_[node_id].activate(std::move(new_node));  // remain gc ??
        return node_id;
    }

    size_t new_node_id() {
        sfdd_nodes_.emplace_back();  // default emplace '0'
        return sfdd_nodes_.size()-1;  // -1 : because just emplace a '0' - for emplace(,)
    	/* 后面再做垃圾回收
        if (avail_.empty()) {
            sfdd_nodes_.emplace_back();  // default emplace '0'
            return sfdd_nodes_.size()-1;  // -1 : because just emplace a '0'
        } else {
            size_t id = avail_.top();
            avail_.pop();
            return id;
        }
        */
    }

    size_t node_array_size() const {
        return sfdd_nodes_.size();
    }
private:
    vector<Sfdd> sfdd_nodes_;  // 存所有的节点
    unordered_map<SfddNode, addr_t> uniq_table_;  // c++11 中的 unordered_map 实现了 hash 存储
    stack<size_t> avail_;  // 这个用来做垃圾回收，留作后面使用
};
}  // namespace sfdd

#endif
