#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <set>
#include <cassert>
#include <algorithm>

using namespace std;

namespace sfdd {

using addr_t = long long int;

// map<int, int> get_index_by_var = {{0,0}};  // x1 stored as "1", x4 stored as "4"...
                                           // indexs of true and false both are "0"

constexpr addr_t SFDD_EMPTY = -1;
constexpr addr_t SFDD_FALSE = -2;
constexpr addr_t SFDD_NULL = -3;

enum NodeType {
	LIT, DECOMP, UNUSED
};

enum OPERATOR_TYPE {
    INTERSECTION,
    XOR,
    AND,
    OR,
};

enum VTREE_TYPE {
    TRIVIAL_TREE,
    RANDOM_TREE,
};


// for exporting as dot
static set<string> node_names;
string check_dec_name(string node_name) {
    if (node_names.find(node_name) != node_names.end()) {
        string new_node_name = \
            node_name.substr(0, node_name.find_last_of('_')+1) + \
            to_string(stoi(node_name.substr(node_name.find_last_of('_')+1, node_name.length()-node_name.find_last_of('_')-1))+1);
        new_node_name = check_dec_name(new_node_name);
        return new_node_name;
    } else {
        node_names.insert(node_name);
        return node_name;
    }
}

inline void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}
}  // namespace sfdd

#endif
