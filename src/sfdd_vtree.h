#ifndef SFDD_VTREE_H
#define SFDD_VTREE_H

#include "define.h"
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <unordered_map>


namespace sfdd {

class Vtree {
public:
    int index = 0;  // index of vtree node. 0: true/false; var start from 1
    int var = 0;  // var number
    Vtree* lt = NULL;
    Vtree* rt = NULL;
    int size = 0;
public:
    Vtree() {}
    Vtree(const int index);
    Vtree(const int index, const int var);
    Vtree(const int parent, const int left_child, const int right_child);
    Vtree(int start_var_index, int end_var_index, std::vector<int> full_order, VTREE_TYPE t = TRIVIAL_TREE);
    Vtree(const Vtree& v);
    Vtree(const std::string& file_name);  // import from file
    Vtree merge(const Vtree& hat);
    ~Vtree();
    Vtree subvtree(int index) const;  // return subvtree whose root is index
    int father_of(int i) const;
    bool is_leaf(int i) const;  // return if node index is leaf
	int get_lca(int a, int b);  // get the lowest common ancestor of two node
    // void print(int indent = 0) const;
    // void print_dot(fstream& out_dot, bool root = false) const;
    // void save_dot_file(const std::string f_name) const;
    void print_vtree(std::fstream& out_dot, bool root = false) const;
    void save_vtree_file(const std::string f_name) const;
};

} // namespace sfdd

#endif
