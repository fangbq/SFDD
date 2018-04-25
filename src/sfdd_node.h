#ifndef SFDD_NODE_H
#define SFDD_NODE_H

#include <vector>
#include <iostream>
#include "define.h"
#include "sfdd_vtree.h"
#include <unordered_set>
#include <type_traits>
#include <functional>

namespace sfdd {

class Manager;

using Element = std::pair<addr_t, addr_t>;

class SfddNode {
public:
    int value = -1;  /* terminal nodes: 0 respects false, 1 respects true; 2*x respects the xth variable (x);
                        2*x+1 respects the negation of the xth variable (-x);
                        nonterminal: -1
                    */
    std::vector<Element> elements;
    int vtree_index = 0;  // respect to vtree node
    int hash_value = 0;
public:
    SfddNode() { elements.clear(); }
    SfddNode(const SfddNode& s) { value = s.value; elements = s.elements; vtree_index = s.vtree_index; }
    ~SfddNode() { value = -1; elements.clear(); vtree_index = 0; }
    SfddNode(int v, int i = 0) { elements.clear(); value = v; vtree_index = i; }
    SfddNode& operator=(const SfddNode& sfdd_node) {
        value = sfdd_node.value;
        elements = sfdd_node.elements;
        vtree_index = sfdd_node.vtree_index;
        return *this;
    }
    bool operator==(const SfddNode& sfdd_node) const;

    bool is_terminal() const;
    bool is_positive() const;
    bool is_negative() const;
    bool is_zero() const;
    bool is_one() const;
    bool is_constant() const;
};


} // namespace sfdd

namespace std {
template <> struct hash<sfdd::SfddNode> {
    std::size_t operator()(const sfdd::SfddNode& n) const {
        size_t h = 0;
        if (n.value >= 0) {
            sfdd::hash_combine(h, hash<int>()(n.value));
            sfdd::hash_combine(h, hash<int>()(n.vtree_index));
            return h;
        } else if (n.value < 0) {
            for (const auto& e : n.elements) {
                sfdd::hash_combine(h, hash<sfdd::addr_t>()(e.first));
                sfdd::hash_combine(h, hash<sfdd::addr_t>()(e.second));
            }
            sfdd::hash_combine(h, hash<int>()(n.vtree_index));
        }
        return h;
    }
};
}


#endif
