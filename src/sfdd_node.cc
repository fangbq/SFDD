#include "sfdd_node.h"
#include "manager.h"

namespace sfdd {

bool SfddNode::operator==(const SfddNode& sfdd_node) const {
// cout << "equals..." << endl;
    if (is_terminal() && sfdd_node.is_terminal()) {
        return value == sfdd_node.value;
    } else if (elements.size() == sfdd_node.elements.size()) {
        return elements == sfdd_node.elements;
    }
    // cout << "no eq" << endl;
    return false;
}

bool SfddNode::is_terminal() const { return value!=-1; }
bool SfddNode::is_positive() const { return value>1 && value%2==0; }
bool SfddNode::is_negative() const { return value>1 && value%2==1; }
bool SfddNode::is_zero() const { return value==0; }
bool SfddNode::is_one() const { return value==1; }
bool SfddNode::is_constant() const { return is_zero() || is_one(); }

// inline bool Element::equals(const Element& e) const {
//     return prime==e.prime && sub==e.sub;
// }

// inline bool Element::operator==(const Element& e) const {
//     return prime==e.prime && sub==e.sub;
// }

// void Element::print(int indent, int counter, const Manager& m) const {
//     for (int i = 0; i < indent; ++i) cout << " ";
//     cout << "E" << counter << "p:" << endl;
//     m.sfdd_nodes_[prime].print(m, indent+1);
//     for (int i = 0; i < indent; ++i) cout << " ";
//     cout << "E" << counter << "s:" << endl;
//     m.sfdd_nodes_[sub].print(m, indent+1);
// }

// void Element::print_dot(fstream& out_dot, int depth, string e_name, const Manager& m) const {
//     out_dot << "\t" << e_name << " [shape=record,label=\"<f0> ";
//     bool prime_out_edge = false;
//     bool sub_out_edge = false;
//     if (m.sfdd_nodes_[prime].is_terminal()) m.sfdd_nodes_[prime].print_dot(out_dot, m);
//     else { out_dot << "●"; prime_out_edge = true; }
//     out_dot << "|<f1> ";
//     if (m.sfdd_nodes_[sub].is_terminal()) m.sfdd_nodes_[sub].print_dot(out_dot, m);
//     else { out_dot << "●"; sub_out_edge = true; }
//     out_dot << "\"]" << endl;
//     ++depth;
//     if (prime_out_edge) {
//         string dec_name = "Dec_" + to_string(depth) + "_1";
//         dec_name = check_dec_name(dec_name);
//         out_dot << "\t" << e_name << ":f0 -> " << dec_name << endl;
//         m.sfdd_nodes_[prime].print_dot(out_dot, m, false, depth, dec_name);
//     }
//     if (sub_out_edge) {
//         string dec_name = "Dec_" + to_string(depth) + "_1";
//         dec_name = check_dec_name(dec_name);
//         out_dot << "\t" << e_name << ":f1 -> " << dec_name << endl;
//         m.sfdd_nodes_[sub].print_dot(out_dot, m, false, depth, dec_name);
//     }
// }

}  // namespace sfdd;
