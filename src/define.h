#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <array>
#include <chrono>

using namespace std;

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

using cache_entry = std::tuple<OPERATOR_TYPE, addr_t, addr_t, addr_t>;

constexpr addr_t SFDD_FALSE = -2;
constexpr addr_t SFDD_EMPTY = -1;
constexpr addr_t SFDD_NULL = -3;

inline void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

// random generator function:
inline int myrandom (int i) { return std::rand()%i;}

#endif
