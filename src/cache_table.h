#ifndef CACHE_TABLE_H_
#define CACHE_TABLE_H_

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include "sfdd_node.h"

namespace sfdd {

class CacheTable {
	using cache_entry = std::tuple<OPERATOR_TYPE, addr_t, addr_t, addr_t>;
public:

    CacheTable() : cache_table_(INIT_SIZE) {}
    CacheTable(const int init_size) : cache_table_(init_size) {}    

	void write_cache(const OPERATOR_TYPE op, const addr_t lhs,
	                 const addr_t rhs, const addr_t res) {
	    auto key = calc_key(op, lhs, rhs);
	    cache_table_[key] = std::make_tuple(op, lhs, rhs, res);
	}

	void clear_cache() {
	    for (auto it =  cache_table_.begin(); it != cache_table_.end(); ++it) {
	        *it = std::make_tuple(OPERATOR_TYPE::NULLOP, -1, -1, -1);
	    }
	}

	addr_t read_cache(const OPERATOR_TYPE op, const addr_t lhs, const addr_t rhs) {
	    auto key = calc_key(op, lhs, rhs);
	    auto res = cache_table_[key];

	    if (std::get<0>(res) == op &&
	        std::get<1>(res) == lhs &&
	        std::get<2>(res) == rhs) {
	        return std::get<3>(res);
	    }
	    return SFDD_NULL;
	}

	size_t calc_key(const OPERATOR_TYPE op, const addr_t lhs,  const addr_t rhs) {
	    size_t key = 0;
	    hash_combine(key, std::hash<int>()(static_cast<int>(op)));
	    hash_combine(key, std::hash<size_t>()(lhs));
	    hash_combine(key, std::hash<size_t>()(rhs));
	    // std::cout << "cache table size: " << cache_table_.size() << std::endl;
	    return key % cache_table_.size();
	}

	void print_cache_table() const {
	    std::cout << "cache_table:-------------------------------" << std::endl;
	    for (auto& x: cache_table_) {
	        std::cout << std::get<0>(x) << std::get<1>(x) << std::get<2>(x) << std::get<3>(x) << std::endl;
	    }
	}
public:
    const unsigned int INIT_SIZE = 1U<<10;
    std::vector<cache_entry> cache_table_;
};

} // namespace sfdd;


#endif
