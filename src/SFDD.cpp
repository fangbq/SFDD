#include "SFDD.h"


Vtree::Vtree(int start_var, int end_var,  VTREE_TYPE t) {
    if (start_var == end_var) {
        size = 1;
        var = start_var;
        return;
    }
    switch (t) {
        case TRIVIAL_TREE:
        {
            // Vtree lv(start_var, (end_var-start_var)/2, t);
            // Vtree rv((end_var-start_var)/2+1, end_var, t);
            // *lt = lv;
            // *rt = rv;
            int mid = start_var+(end_var-start_var)/2;
            lt = new Vtree(start_var, mid, t);
            rt = new Vtree(mid+1, end_var, t);
            size = lt->size + rt->size;
            break;
        }
        case RANDOM_TREE:
        {
            break;
        }
        default:
            ;
    }
    return;
}

void Vtree::print(int indent) const {
    if (indent == 0)
        cout << "Vtree Size: " << size << endl;
    for (int i = 0; i < indent; ++i) cout << " ";
    if (var == -1) cout << "¬" << endl;
    else cout << var << endl;
    if (lt) lt->print(indent+1);
    if (rt) rt->print(indent+1);
    return;
}


SFDD Element::get_value() const {
    return prime.Conjoin(sub);
}


SFDD SFDD::Xor(const SFDD& sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::Conjoin(const SFDD& sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::Disjoin(const SFDD& sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}


SFDD Manager::sfddVar(int i) const {
    SFDD sfdd;
    return sfdd;
}

SFDD Manager::sfddZero() const {
    SFDD sfdd;
    return sfdd;
}

SFDD Manager::sfddOne() const {
    SFDD sfdd;
    return sfdd;
}
