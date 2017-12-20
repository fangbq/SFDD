#ifndef SFDD_NODETABLE_H
#define SFDD_NODETABLE_H

#include "define.h"

class SfddNodeTable {
private:
	vector<SFDD> sfdd_nodes_;
	unordered_map<SFDD, addr_t> uniq_table_;
    stack<size_t> avail_;
public:

    SfddNodeTable() :
        sfdd_nodes_(),
        uniq_table_(),
        avail_() {}
};

#endif

