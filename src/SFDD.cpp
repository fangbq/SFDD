#include "SFDD.h"


Vtree::Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t) {
    if (start_var_index == end_var_index) {
        size = 1;
        var = full_order[start_var_index-1];
        return;
    }
    switch (t) {
        case TRIVIAL_TREE:
        {
            int mid = start_var_index+(end_var_index-start_var_index)/2;
            index = mid;
            lt = new Vtree(start_var_index, mid, full_order, t);
            rt = new Vtree(mid+1, end_var_index, full_order, t);
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

set<int> Vtree::get_variables() const {
    set<int> vars;
    if (var) {
        vars.insert(var);
        return vars;
    }
    if (lt) {
        set<int> lvars = lt->get_variables();
        vars.insert(lvars.begin(), lvars.end());
    }
    if (rt) {
        set<int> rvars = rt->get_variables();
        vars.insert(rvars.begin(), rvars.end());
    }
    return vars;
}

void Vtree::print(int indent) const {
    if (indent == 0)
        cout << "Vtree Size: " << size << endl;
    for (int i = 0; i < indent; ++i) cout << " ";
    if (!var) cout << index << endl;
    else cout << "x" << var << endl;
    if (lt) lt->print(indent+1);
    if (rt) rt->print(indent+1);
    return;
}

void Vtree::print_dot(fstream & out_dot, bool root, int depth, string dec_name) const {
    // if (root) {
    //     out_dot << "digraph G {" << endl;
    //     if (zero() | one()) {
    //         out_dot << "\t" << value << " [shape=record, label=\"" \
    //         << value << "\"]" << endl << "}";
    //         return;
    //     }
    // }
    // for (int i = 0; i < indent; ++i) cout << " ";
    // if (!var) out_dot << index << endl;
    // else out_dot << "x" << var << endl;
    // if (lt) lt->print(indent+1);
    // if (rt) rt->print(indent+1);
    // if (root) out_dot << "}" << endl;
    return;
}

int SFDD::size() const {
    if (empty()) return 0;
    if (terminal()) return 1;

    int size = 0;
    for (vector<Element>::const_iterator e = elements.begin(); \
    e != elements.end(); ++e) {
        size += e->prime.size();
        size += e->sub.size();
    }
    return size;
}

bool SFDD::equals(const SFDD & sfdd) const {
    if (terminal() && sfdd.terminal()) {
        return value == sfdd.value;
    } else if (size() == sfdd.size()) {
        set<int> equaled_elements;
        for (vector<Element>::const_iterator e1 = elements.begin(); \
        e1 != elements.end(); ++e1) {
            bool found_equivalent = false;
            for (int i = 0; i < sfdd.size(); ++i) {
                if (equaled_elements.find(i) == equaled_elements.end()
                && e1->equals(sfdd.elements[i])) {
                    equaled_elements.insert(i);
                    found_equivalent = true;
                    break;
                }
            }
            if (!found_equivalent) return false;
        }
        return true;
    }
    return false;
}

SFDD& SFDD::reduced(Manager & m) {
    if (terminal()) return *this;
    bool valid = false;
    for (vector<Element>::iterator e = elements.begin(); \
    e != elements.end(); ) {
        // 1.1 removes those elements that primes are false
        if (e->prime.reduced(m).zero()) {
            e = elements.erase(e);
        } else if (!e->sub.reduced(m).zero()) {
            valid = true;
            ++e;
        } else {
            ++e;
        }
    }
    // 1.2 return false if all elements' subs are false
    if (!valid) elements.clear();
    if (size()==0) value = 0;
    // 2 compressing
    for (vector<Element>::iterator e1 = elements.begin(); \
    e1 != elements.end(); ) {
        bool is_delete = false;
        for (vector<Element>::iterator e2 = elements.begin(); \
        e2 != elements.end(); ++e2) {
            if (e1 != e2 && e1->sub.equals(e2->sub)) {
                is_delete = true;
                e2->prime = e2->prime.Xor(e1->prime, m).reduced(m);
                e1 = elements.erase(e1);
                break;
            }
        }
        if (!is_delete)
            ++e1;
    }
    return *this;
}

SFDD SFDD::expanded(Manager & m) const {
    if (value<2) {
        // return {(1, 1), (x, 0)} if 1; return {(1, 0), (x, 0)} if 0
        // SFDD expanded_sfdd;
        // expanded_sfdd.vtree_index = vtree_index;
        // Element e1, e2;
        // e1.prime.value = 1;
        // e1.sub.value = 1;
        // expanded_sfdd.elements.push_back(e1);
        // return expanded_sfdd;
    }
    // return self if non-constant
    return *this;
}

SFDD SFDD::Intersection(const SFDD & sfdd, Manager & m) const {
    // cout << "Intersection..." << endl;
    SFDD new_sfdd;
    new_sfdd.vtree_index = vtree_index;
    if (zero()) return *this;
    if (sfdd.zero()) return sfdd;
    if (terminal() && sfdd.terminal()) {
        // base case
        if (equals(sfdd) || sfdd.negative())
            new_sfdd = *this;
        else if (negative())
            new_sfdd = sfdd;
        else
            new_sfdd.value = 0;
    } else if (vtree_index == sfdd.vtree_index) {
        SFDD expanded_sfdd1 = expanded(m);
        SFDD expanded_sfdd2 = sfdd.expanded(m);
        for (vector<Element>::const_iterator e1 = expanded_sfdd1.elements.begin();
        e1 != expanded_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = expanded_sfdd2.elements.begin();
            e2 != expanded_sfdd2.elements.end(); ++e2) {
                Element new_e;
                new_e.prime = e1->prime.Intersection(e2->prime, m).reduced(m);
                if (!new_e.prime.zero()) {
                    new_e.sub = e1->sub.Intersection(e2->sub, m).reduced(m);
                    new_sfdd.elements.push_back(new_e);
                }
            }
        }
    } else {
        cout << "Intersection Error!" << endl;
        // SFDD outer_sfdd, inner_sfdd;
        // if (vtree_index > sfdd.vtree_index) {
        //     outer_sfdd = expanded(m);
        //     inner_sfdd = sfdd.expanded(m);
        // } else {
        //     outer_sfdd = sfdd.expanded(m);
        //     inner_sfdd = expanded(m);
        // }
        // for (vector<Element>::const_iterator outer_e = outer_sfdd.elements.begin();
        // outer_e != outer_sfdd.elements.end(); ++outer_e) {
        //     Element new_e;
        //     new_e.prime = outer_e->prime.Intersection(inner_sfdd, m).reduced(m);
        //     new_e.sub = outer_e->sub;
        //     new_sfdd.elements.push_back(new_e);
        // }
    }
    // new_sfdd.reduced(m).print();
    return new_sfdd.reduced(m);
}

SFDD SFDD::Xor(const SFDD & sfdd, Manager & m) const {
    // cout << "Xor..." << endl;
    if (empty()) return sfdd;
    if (sfdd.empty()) return *this;

    if (zero()) return sfdd;
    if (sfdd.zero()) return *this;
    SFDD new_sfdd;
    if (terminal() && sfdd.terminal()) {
        // base case
        if (equals(sfdd)) {
            new_sfdd.value = 0;
        } else if (zero() || sfdd.zero()) {
            new_sfdd.value = value+sfdd.value;
        } else if (one()) {
            new_sfdd.value = sfdd.value/2*2+1-sfdd.value%2;
        } else if (sfdd.one()) {
            new_sfdd.value = value/2*2+1-value%2;
        } else {
            new_sfdd.value = 1;
        }
    } else if (vtree_index == sfdd.vtree_index) {
        new_sfdd.vtree_index = vtree_index;
        SFDD expanded_sfdd1 = expanded(m);
        SFDD expanded_sfdd2 = sfdd.expanded(m);
        for (vector<Element>::const_iterator e1 = expanded_sfdd1.elements.begin();
        e1 != expanded_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = expanded_sfdd2.elements.begin();
            e2 != expanded_sfdd2.elements.end(); ++e2) {
                Element new_e;
                new_e.prime = e1->prime.Intersection(e2->prime, m).reduced(m);
                if (!new_e.prime.zero()) {
                    new_e.sub = e1->sub.Xor(e2->sub, m).reduced(m);
                    new_sfdd.elements.push_back(new_e);
                }
            }
        }
    } else {
        cout << "Xor Error!" << endl;
    }
    return new_sfdd.reduced(m);
}

SFDD SFDD::And(const SFDD & sfdd, Manager & m) const {
    // cout << "And..." << endl;
    if (empty()) return sfdd;
    if (sfdd.empty()) return *this;

    if (zero()) return *this;
    if (sfdd.zero()) return sfdd;
    if (one()) return sfdd;
    if (sfdd.one()) return *this;

    SFDD new_sfdd;
    if (terminal() && sfdd.terminal()) {
        // base case
        if (zero() || sfdd.zero()) {
            new_sfdd.value = 0;
        } else if (one() || sfdd.one()) {
            new_sfdd.value = value*sfdd.value;
        } else if (equals(sfdd)) {
            new_sfdd = *this;
        } else {
            new_sfdd.value = 0;
        }
    } else if (vtree_index == sfdd.vtree_index) {
        SFDD expanded_sfdd1 = expanded(m);
        SFDD expanded_sfdd2 = sfdd.expanded(m);
        for (vector<Element>::const_iterator e1 = expanded_sfdd1.elements.begin();
        e1 != expanded_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = expanded_sfdd2.elements.begin();
            e2 != expanded_sfdd2.elements.end(); ++e2) {
                SFDD prime_product = e1->prime.And(e2->prime, m).reduced(m);
                SFDD sub_product = e1->sub.And(e2->sub, m).reduced(m);
                SFDD tmp_new_sfdd;
                for (vector<Element>::const_iterator new_e = new_sfdd.elements.begin();
                new_e != new_sfdd.elements.end(); ++new_e) {
                    Element inter_e, origin_e;
                    inter_e.prime = new_e->prime.Intersection(prime_product, m);
                    if (!inter_e.prime.zero()) {
                        inter_e.sub = new_e->sub.Xor(sub_product, m);
                        tmp_new_sfdd.elements.push_back(inter_e);  // add 1
                        origin_e.prime = new_e->prime.Xor(inter_e.prime, m);
                        origin_e.sub = new_e->sub;
                        tmp_new_sfdd.elements.push_back(origin_e);  // add 2
                        prime_product = prime_product.Xor(inter_e.prime, m);
                    } else {
                        tmp_new_sfdd.elements.push_back(*new_e);
                    }
                }
                if (!prime_product.zero()) {
                    Element last_e;  // last element that has removed all common parts with others
                    last_e.prime = prime_product;
                    last_e.sub = sub_product;
                    tmp_new_sfdd.elements.push_back(last_e);  // add last element
                }
                new_sfdd = tmp_new_sfdd;
            }
        new_sfdd.vtree_index = vtree_index;
        }
    } else {
        cout << "And Error!" << endl;
    }
    return new_sfdd.reduced(m);
}

SFDD SFDD::Or(const SFDD & sfdd, Manager & m) const {
    // cout << "Or..." << endl;
    if (empty()) return sfdd;
    if (sfdd.empty()) return *this;

    if (zero()) return sfdd;
    if (sfdd.zero()) return *this;
    if (one()) return *this;
    if (sfdd.one()) return sfdd;

    return Xor(sfdd, m).Xor(And(sfdd, m), m);
}

void SFDD::print(int indent) const {
    if (elements.empty()) {
        for (int i = 0; i < indent; ++i) cout << " ";
        if (value < 2) {
            cout << value << endl;
        } else {
            if (negative()) cout << "-";
            cout << "x" << value/2 << endl;
        }
        return;
    }
    for (int i = 0; i < indent; ++i) cout << " ";
    cout << "Dec " << vtree_index << ":" << endl;
    int counter = 1;
    for (vector<Element>::const_iterator e = elements.begin();
    e != elements.end(); ++e) {
        e->print(indent+1, counter++);
    }
    return;
}

static set<string> node_names;

string check_dec_name(string node_name) {
    // cout << "haha " << node_name << endl;
    // cout << "----------" << endl;
    // for (set<string>::iterator nn = node_names.begin(); nn != node_names.end(); ++nn)
    //     cout << *nn << endl;
    // cout << "===================" << endl;
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

void SFDD::print_dot(fstream & out_dot, bool root, int depth, string dec_name) const {
    if (root) {
        out_dot << "digraph G {" << endl;
        if (zero() | one()) {
            out_dot << "\t" << value << " [shape=record, label=\"" \
            << value << "\"]" << endl << "}";
            return;
        }
    }
    if (elements.empty()) {
        if (value < 2) {
            out_dot << value;
        } else {
            if (negative()) out_dot << "-";
            out_dot << "x" << value/2;
        }
        return;
    }
    ++depth;
    out_dot << "\t" << dec_name << " [shape=circle, label=\"" << vtree_index << "\"]" << endl;
    string e_name = "Ele_" + to_string(depth) + "_1";
    for (vector<Element>::const_iterator e = elements.begin();
    e != elements.end(); ++e) {
        e_name = check_dec_name(e_name);
        out_dot << "\t" << dec_name << " -> " << e_name << endl;
        e->print_dot(out_dot, depth, e_name);
    }
    if (root) out_dot << "}" << endl;
}

void Element::print(int indent, int counter) const {
    for (int i = 0; i < indent; ++i) cout << " ";
    cout << "E" << counter << "p:" << endl;
    prime.print(indent+1);
    for (int i = 0; i < indent; ++i) cout << " ";
    cout << "E" << counter << "s:" << endl;
    sub.print(indent+1);
}

void Element::print_dot(fstream & out_dot, int depth, string e_name) const {
    out_dot << "\t" << e_name << " [shape=record,label=\"<f0> ";
    bool prime_out_edge = false;
    bool sub_out_edge = false;
    if (prime.terminal()) prime.print_dot(out_dot);
    else { out_dot << "●"; prime_out_edge = true; }
    out_dot << "|<f1> ";
    if (sub.terminal()) sub.print_dot(out_dot);
    else { out_dot << "●"; sub_out_edge = true; }
    out_dot << "\"]" << endl;
    ++depth;
    if (prime_out_edge) {
        string dec_name = "Dec_" + to_string(depth) + "_1";
        dec_name = check_dec_name(dec_name);
        out_dot << "\t" << e_name << ":f0 -> " << dec_name << endl;
        prime.print_dot(out_dot, false, depth, dec_name);
    }
    if (sub_out_edge) {
        string dec_name = "Dec_" + to_string(depth) + "_1";
        dec_name = check_dec_name(dec_name);
        out_dot << "\t" << e_name << ":f1 -> " << dec_name << endl;
        sub.print_dot(out_dot, false, depth, dec_name);
    }
}

SFDD Manager::sfddZero() {
    return get_SFDD1(vtree, 0).reduced(*this);
}

SFDD Manager::sfddOne() {
    return get_SFDD1(vtree, 1).reduced(*this);
}

SFDD Manager::sfddVar(const int tmp_var) {
    assert(tmp_var != 0);
    if (tmp_var < 0) return get_SFDD1(vtree, (0-tmp_var)*2+1).reduced(*this);
    else if (tmp_var > 0) return get_SFDD1(vtree, tmp_var*2).reduced(*this);
    else return get_SFDD1(vtree, tmp_var).reduced(*this);
}

extern SFDD get_SFDD1(const Vtree* v, const int var) {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // check if constant
    if (v->var) {
        sfdd.value = var;  /* different with get_SFDD, sfddVar \
                            solves 'f=x_i', so the var only be \
                            the terminal. */
        return sfdd;
    }
    Element e1, e2;
    set<int> lt_vars = v->lt->get_variables();
    set<int> rt_vars = v->rt->get_variables();
    if (var == 0) {
        // rule 1.(d)
        e1.prime = get_SFDD2(v->lt, 0);
        e1.sub = get_SFDD1(v->rt, 0);
        sfdd.elements.push_back(e1);
        return sfdd;
    } else if (var == 1) {
        // rule 1.(c)
        e1.prime = get_SFDD1(v->lt, 1);
        e1.sub = get_SFDD1(v->rt, 1);  // normalization
        e2.prime = get_SFDD2(v->lt, 1);
        e2.sub = get_SFDD1(v->rt, 0);
    } else if (lt_vars.find(var/2) != lt_vars.end()) {
        // rule 1.(a)
        e1.prime = get_SFDD1(v->lt, var);  // normalization
        e1.sub = get_SFDD1(v->rt, 1);
        e2.prime = get_SFDD2(v->lt, var);
        e2.sub = get_SFDD1(v->rt, 0);
    } else {
        // rule 1.(b)
        e1.prime = get_SFDD1(v->lt, 1);
        e1.sub = get_SFDD1(v->rt, var);  // normalization
        e2.prime = get_SFDD2(v->lt, 1);
        e2.sub = get_SFDD1(v->rt, 0);
    }
    sfdd.elements.push_back(e1);
    sfdd.elements.push_back(e2);
    return sfdd;
}

/*
 * @para: v - vtree, rht - in compiling rules 2 and 3, there
 *        are formulas like $f \oplus g$, rht means 'g' in
 *        this kind format.
 */
extern SFDD get_SFDD2(const Vtree* v, const int rht) {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // check if constant
    if (v->var) {
        if (rht == 1) sfdd.value = v->var*2;
        else if (rht == 0) sfdd.value = v->var*2+1;
        else if (rht%2 == 0) sfdd.value = 1;
        else sfdd.value = 0;
        return sfdd;
    }
    Element e1, e2;
    set<int> lt_vars = v->lt->get_variables();
    set<int> rt_vars = v->rt->get_variables();
    if (rht == 0) {
        // rule 2.(d)
        e1.prime = get_SFDD2(v->lt);
        e1.sub = get_SFDD2(v->rt);
        sfdd.elements.push_back(e1);
        return sfdd;
    } else if (rht == 1) {
        // rule 2.(c)
        e1.prime = get_SFDD1(v->lt, 1);
        e1.sub = get_SFDD2(v->rt, 1);  // normalization
        e2.prime = get_SFDD2(v->lt, 1);
        e2.sub = get_SFDD2(v->rt);
    } else if (lt_vars.find(rht/2) != lt_vars.end()) {
        // rule 2.(a)
        e1.prime = get_SFDD1(v->lt, rht);  // normalization
        e1.sub = get_SFDD2(v->rt, 1);
        e2.prime = get_SFDD2(v->lt, rht);
        e2.sub = get_SFDD2(v->rt);
    } else {
        // rule 2.(b)
        e1.prime = get_SFDD1(v->lt, 1);
        e1.sub = get_SFDD2(v->rt, rht);  // normalization
        e2.prime = get_SFDD2(v->lt, 1);
        e2.sub = get_SFDD2(v->rt);
    }
    sfdd.elements.push_back(e1);
    sfdd.elements.push_back(e2);
    return sfdd;
}
