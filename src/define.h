#ifndef DEFINE_H
#define DEFINE_H


#include <unordered_set>
#include <map>
#include <cassert>
#include <chrono>
#include <utility>

namespace sfdd {

enum OPERATOR_TYPE {
	NULLOP,
	INTER,
    XOR,
    AND,
    OR
};

enum VTREE_TYPE {
    TRIVIAL_TREE,
    RANDOM_TREE
};

using addr_t = long long int;

constexpr addr_t SFDD_FALSE = -2;
constexpr addr_t SFDD_EMPTY = -1;
constexpr addr_t SFDD_NULL = -3;

inline void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

// random generator function:
inline int myrandom (int i) { return std::rand()%i;}

} // namespace sfdd

#endif
