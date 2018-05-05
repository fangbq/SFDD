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


}  // namespace sfdd;
