#include "sfdd_vtree.h"
#include <algorithm>
#include <stack>
#include <fstream>
#include <sstream>
#include <assert.h>


extern std::map<int, int> get_index_by_var;


namespace sfdd {

Vtree::Vtree(const int i) :
    index(i), size(1) {}

Vtree::Vtree(const int i, const int v) :
    index(i), var(v), size(1) {}

Vtree::Vtree(const int parent, const int left_child, const int right_child) {
    index = parent;
    lt = new Vtree(left_child);
    rt = new Vtree(right_child);
    size = 3;
}

Vtree::Vtree(int start_var_index, int end_var_index, std::vector<int> full_order, VTREE_TYPE t) {
    if (start_var_index == end_var_index) {
        size = 1;
        index = start_var_index;
        var = full_order[(start_var_index-1)/2];
        get_index_by_var[var] = start_var_index;
        return;
    }
    switch (t) {
        case RANDOM_TREE:
        {
            int mid = myrandom(end_var_index-start_var_index)+start_var_index+1;
            mid = mid%2 ? mid-1 : mid;
            index = mid;
            // cout << mid << endl;
            lt = new Vtree(start_var_index, mid-1, full_order, t);
            rt = new Vtree(mid+1, end_var_index, full_order, t);
            size = lt->size + rt->size + 1;
            break;
        }
        case TRIVIAL_TREE:
        {
            int mid = (start_var_index+end_var_index)/2;
            mid = mid%2 ? mid-1 : mid;
            index = mid;
            // std::cout << mid << std::endl;
            lt = new Vtree(start_var_index, mid-1, full_order, t);
            rt = new Vtree(mid+1, end_var_index, full_order, t);
            size = lt->size + rt->size + 1;
            break;
        }
        default:
            std::cerr << "vtree error 2" << std::endl;
            ;
    }
    return;
}

Vtree::Vtree(const Vtree& v)
    : index(v.index), var(v.var), lt(NULL), rt(NULL), size(v.size) {
    if (v.lt) lt = new Vtree(*v.lt);
    if (v.rt) rt = new Vtree(*v.rt);
}

Vtree::Vtree(const std::string& file_name) {
    std::ifstream ifs(file_name, std::ios::in);
    std::string line;

    while (getline(ifs, line)) {
        if (line[0] == 'c') continue;
        size = stoi(line.substr(6, line.length()-6));
        break;
    }

    // reading vtree nodes from file
    char node_type = ' ';
    int left_child, right_child, parent, var;
    std::stack<Vtree> vtree_nodes;
    for (int i = 0; i < size; ++i) {
        ifs >> node_type;
        if (node_type == 'L') {
            ifs >> parent >> var;
            vtree_nodes.push(*new Vtree(++parent, var));
        } else if (node_type == 'I') {
            ifs >> parent >> left_child >> right_child;
            vtree_nodes.push(*new Vtree(++parent, ++left_child, ++right_child));
        } else {
            std::cerr << "reading vtree file, node type input error" << std::endl;
        }
    }

    // constructing the big vtree from vtree_nodes
    index = vtree_nodes.top().index;
    var = vtree_nodes.top().var;
    size = vtree_nodes.top().size;
    lt = new Vtree(*vtree_nodes.top().lt);
    rt = new Vtree(*vtree_nodes.top().rt);
    vtree_nodes.pop();
    while (!vtree_nodes.empty()) {
        merge(vtree_nodes.top());
        vtree_nodes.pop();
    }
}

// for reading vtree from a *.vtree file
Vtree Vtree::merge(const Vtree& hat) {
    if (index == hat.index) {
        if (hat.size == 1) {   
            var = hat.var;
            get_index_by_var[var] = index;
        }
        else {
            lt = new Vtree(hat.lt->index);
            rt = new Vtree(hat.rt->index);
        }
    } else if (index < hat.index) {
        rt->merge(hat);
    } else if (index > hat.index) {
        lt->merge(hat);
    } else {
        std::cerr << "Vtree merge error!!!" << std::endl;
    }
    size = 1 + (lt ? lt->size : 0) + (rt ? rt->size : 0);
    return *this;
}

Vtree::~Vtree() {
    if (lt != NULL) { delete lt; lt = NULL; }
    if (rt != NULL) { delete rt; rt = NULL; }
}

Vtree Vtree::subvtree(int i) const {
    // assert(i > 0);
    if (i == index) return *this;
    else if (i < index) return lt->subvtree(i);
    else return rt->subvtree(i);
}

int Vtree::father_of(int i) const {
    assert(i > 0);
    if (lt->index == i || rt->index == i) return index;
    if (i < index) return lt->father_of(i);
    else if (i > index) return rt->father_of(i);
    else {
        std::cerr << "father_of error!" << std::endl;
        exit(1);
    }
}

bool Vtree::is_leaf(int i) const {
    if (i == 0) return true;  // value is true or false when vtree_index equals zero
    return subvtree(i).lt == NULL;
}

int Vtree::get_lca(int a, int b) {
// cout << "get_lca..." << endl;
    // if (!&v) return 0;
    // cout << a << " " << v.index << " " << b << endl;
    if (index == a || index == b) return index;
    int L = lt ? lt->get_lca(a, b) : 0;
    int R = rt ? rt->get_lca(a, b) : 0;
    if (L && R) return index;  // if p and q are on both sides
    return L ? L : R;  // either one of p,q is on one side OR p,q is not in L&R subtrees
}

// void Vtree::print(int indent) const {
//     if (indent == 0)
//         cout << "Vtree Size: " << size << endl;
//     for (int i = 0; i < indent; ++i) cout << " ";
//     cout << index << "  x"+to_string(var) << endl;
//     // cout << this << endl;
//     if (lt) lt->print(indent+1);
//     if (rt) rt->print(indent+1);
//     return;
// }

// void Vtree::print_dot(fstream& out_dot, bool root) const {
//     if (root) out_dot << "graph {" << endl;
//     out_dot << "\ti" << index;
//     if (var) out_dot << "x"+to_string(var);
//     out_dot << " [shape=none, label=\"" << index;
//     if (var) out_dot << "_x"<< var;
//     out_dot << "\"]" << endl;
//     if (lt) {
//         out_dot << "\ti" << index << " -- i";
//         out_dot << lt->index;
//         if (lt->var) out_dot << "x"+to_string(lt->var);
//         out_dot << endl;
//         lt->print_dot(out_dot, false);
//     }
//     if (rt) {
//         out_dot << "\ti" << index << " -- i";
//         out_dot <<  rt->index;
//         if (rt->var) out_dot << "x"+to_string(rt->var);
//         out_dot << endl;
//         rt->print_dot(out_dot, false);
//     }
//     if (root) out_dot << "}" << endl;
//     return;
// }

void Vtree::print_vtree(std::fstream& out_dot, bool root) const {
    if (root) out_dot << "vtree " << size << std::endl;
    if (lt) lt->print_vtree(out_dot, false);
    if (rt) rt->print_vtree(out_dot, false);
    if (var) out_dot << "L " << index-1 << " " << var << std::endl;
    else out_dot << "I " << index-1 << " " << lt->index-1 << " " << rt->index-1 << std::endl;
}

// void Vtree::save_dot_file(const std::string f_name) const {
//     fstream f;
//     f.open(f_name, fstream::out | fstream::trunc);
//     print_dot(f, true);
//     f.close();
// }

void Vtree::save_vtree_file(const std::string f_name) const {
    std::fstream f;
    f.open(f_name, std::fstream::out | std::fstream::trunc);
    f << "c ids of vtree nodes start at 0" << std::endl;
    f << "c ids of variables start at 1" << std::endl;
    f << "c vtree nodes appear bottom-up, children before parents" << std::endl;
    f << "c" << std::endl;
    f << "c file syntax:" << std::endl;
    f << "c vtree number-of-nodes-in-vtree" << std::endl;
    f << "c L id-of-leaf-vtree-node id-of-variable" << std::endl;
    f << "c I id-of-internal-vtree-node id-of-left-child id-of-right-child" << std::endl;
    f << "c" << std::endl;
    print_vtree(f, true);
    f.close();
}

} // namespace sfdd
