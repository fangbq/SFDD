#include "SFDD.h"


map<int, int> get_index_by_var;

Vtree::Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t) {
    if (start_var_index == end_var_index) {
        size = 1;
        index = start_var_index;
        var = full_order[(start_var_index-1)/2];
        get_index_by_var[var] = start_var_index;
        return;
    }
    switch (t) {
        case TRIVIAL_TREE:
        {
            int mid = (end_var_index+start_var_index)/2;
            mid = mid%2 ? mid-1 : mid;
            index = mid;
            lt = new Vtree(start_var_index, mid-1, full_order, t);
            rt = new Vtree(mid+1, end_var_index, full_order, t);
            size = lt->size + rt->size + 1;
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

Vtree::Vtree(const Vtree & v)
    : index(v.index), var(v.var), size(v.size) {
    if (v.lt) lt = new Vtree(*v.lt);
    if (v.rt) rt = new Vtree(*v.rt);
}

Vtree* Vtree::subvtree(int i) {
/*    Vtree* tmp_v = this;
    while (tmp_v->index != i) {
        if (tmp_v->index > i) tmp_v = tmp_v->lt;
        else tmp_v = tmp_v->rt;
    }
    return tmp_v;*/
    assert(i > 0);
    if (i == index) return this;
    else if (i < index) return lt->subvtree(i);
    else return rt->subvtree(i);
}

bool Vtree::leaf(int index) {
    if (index == 0) return true;  // value is true or false when vtree_index equals zero
    return subvtree(index)->lt == NULL;
}

/*
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
*/
void Vtree::print(int indent) const {
    if (indent == 0)
        cout << "Vtree Size: " << size << endl;
    for (int i = 0; i < indent; ++i) cout << " ";
    cout << index << "  x"+to_string(var) << endl;
    if (lt) lt->print(indent+1);
    if (rt) rt->print(indent+1);
    return;
}

void Vtree::print_dot(fstream & out_dot, bool root) const {
    if (root) out_dot << "graph {" << endl;
    out_dot << "\ti" << index;
    if (var) out_dot << "x"+to_string(var);
    out_dot << " [shape=none, label=\"" << index;
    if (var) out_dot << "_x"<< var;
    out_dot << "\"]" << endl;
    if (lt) {
        out_dot << "\ti" << index << " -- i";
        out_dot << lt->index;
        if (lt->var) out_dot << "x"+to_string(lt->var);
        out_dot << endl;
        lt->print_dot(out_dot, false);
    }
    if (rt) {
        out_dot << "\ti" << index << " -- i";
        out_dot <<  rt->index;
        if (rt->var) out_dot << "x"+to_string(rt->var);
        out_dot << endl;
        rt->print_dot(out_dot, false);
    }
    if (root) out_dot << "}" << endl;
    return;
}

void Vtree::save_file_as_dot(const string f_name) const {
    fstream f;
    f.open("dotG/test0/"+f_name+".dot", fstream::out | fstream::trunc);
    print_dot(f, true);
    f.close();
}

int SFDD::size() const {
    if (empty()) return 0;
    if (terminal()) return 1;

    int size = elements.size();
    for (vector<Element>::const_iterator e = elements.begin(); \
    e != elements.end(); ++e) {
        size += e->prime.size();
        size += e->sub.size();
    }
    return size;
}

bool SFDD::computable_with(const SFDD & sfdd, const Manager & m) const {
    m.vtree->leaf(normalized_sfdd1.vtree_index) || !m.vtree->leaf(normalized_sfdd2.vtree_index);
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

SFDD& SFDD::reduced(const Manager & m) {
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
    //3 trimming
    if (elements.size() == 1) {
        // {(-a, T)} -> -a
        if (elements[0].prime.terminal() && elements[0].sub.one()) {
            value = elements[0].prime.value;
            elements.clear();
        }
    }
    if (elements.size() == 2) {
        // {(a, 1), (other-pi-terms/a, 0)} -> a
        // {(1, a), (other-pi-terms/1, 0)} -> a
        if (elements[0].sub.zero()) {
            if (elements[1].sub.one() && elements[1].prime.terminal()) {
                value = elements[1].prime.value;
                elements.clear();
            } else if (elements[1].prime.one() && elements[1].sub.terminal()) {
                value = elements[1].sub.value;
                elements.clear();
            }
        } else if (elements[1].sub.zero()) {
            if (elements[0].sub.one() && elements[0].prime.terminal()) {
                value = elements[0].prime.value;
                elements.clear();
            } else if (elements[0].prime.one() && elements[0].sub.terminal()) {
                value = elements[0].sub.value;
                elements.clear();
            }
        }
    }
    return *this;
}

extern int get_lca(int a, int b, Vtree * v) {
    if (!v) return 0;
    if (v->index == a || v->index == b) return v->index;
    int L = get_lca(a, b, v->lt);
    int R = get_lca(a, b, v->rt);
    if (L && R) return v->index;  // if p and q are on both sides
    return L ? L : R;  // either one of p,q is on one side OR p,q is not in L&R subtrees
}

// int level = 1;
SFDD& SFDD::normalized(int lca, const Manager & m) {
    if (!terminal()) value = -1;
    // normalize \alpha from vtree_index to bottom
    // int c = 1;
    for (vector<Element>::iterator e = elements.begin(); \
    e != elements.end(); ++e) {
        // if (level < 4) e->prime.save_file_as_dot(to_string(level)+"before_prime"+to_string(c));
        e->prime.normalized(m.vtree->subvtree(vtree_index)->lt->index, m);
        // if (level < 4) e->prime.save_file_as_dot(to_string(level)+"prime"+to_string(c));
        // if (level < 4) e->sub.save_file_as_dot(to_string(level)+"before_sub"+to_string(c));
        e->sub.normalized(m.vtree->subvtree(vtree_index)->rt->index, m);
        // if (level < 4) e->sub.save_file_as_dot(to_string(level++)+"sub"+to_string(c++));
    }
// m.vtree->subvtree(lca)->save_file_as_dot("z_vtree");
// this->save_file_as_dot("z_sfdd");
//     cout << "hh 2" << endl;
    // top-down from lca to vtree_index
    *this = normalization_1(m.vtree->subvtree(lca), *this, m);
    return *this;
}

SFDD SFDD::Intersection(const SFDD & sfdd, const Manager & m) const {
    // cout << "Intersection..." << endl;
    if (zero()) return *this;
    if (sfdd.zero()) return sfdd;
            // cout << "==================================" << endl;
            // cout << "index: " << vtree_index << endl;
            // print();
            // cout << "inter with -------------" << endl;
            // cout << "index: " << sfdd.vtree_index << endl;
            // sfdd.print();
    // normalizing for both sides
    SFDD normalized_sfdd1 = *this, normalized_sfdd2 = sfdd;
    if ((!m.vtree->leaf(normalized_sfdd1.vtree_index) || !m.vtree->leaf(normalized_sfdd2.vtree_index)) \
        && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, m.vtree);
        normalized_sfdd1.normalized(lca, m);
        normalized_sfdd2.normalized(lca, m);
    }

    SFDD new_sfdd;
    new_sfdd.vtree_index = normalized_sfdd1.vtree_index;

    if (normalized_sfdd1.terminal() && normalized_sfdd2.terminal()) {
        // base case
        if (normalized_sfdd1.equals(normalized_sfdd2) || normalized_sfdd2.negative())
            new_sfdd = normalized_sfdd1;
        else if (normalized_sfdd1.negative())
            new_sfdd = normalized_sfdd2;
        else
            new_sfdd.value = 0;
    } else {
        for (vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
        e1 != normalized_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
            e2 != normalized_sfdd2.elements.end(); ++e2) {
                Element new_e;
                new_e.prime = e1->prime.Intersection(e2->prime, m);
                if (!new_e.prime.zero()) {
                    new_e.sub = e1->sub.Intersection(e2->sub, m);
                    new_sfdd.elements.push_back(new_e);
                }
            }
        }
    }

            // cout << "get ------------------" << endl;
            // new_sfdd.print();
    return new_sfdd.reduced(m);
}

//int c = 1;
SFDD SFDD::Xor(const SFDD & sfdd, const Manager & m) const {
    cout << "Xor..." << endl;
    if (empty()) return sfdd;
    if (sfdd.empty()) return *this;

    if (zero()) return sfdd;
    if (sfdd.zero()) return *this;

    SFDD normalized_sfdd1 = *this, normalized_sfdd2 = sfdd;
    if ((!m.vtree->leaf(normalized_sfdd1.vtree_index) || !m.vtree->leaf(normalized_sfdd2.vtree_index)) \
        && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, m.vtree);
        normalized_sfdd1.normalized(lca, m);
        // normalized_sfdd1.save_file_as_dot("sfdd1 "+to_string(c));
        normalized_sfdd2.normalized(lca, m);
        // normalized_sfdd2.save_file_as_dot("sfdd2 "+to_string(c++));
    }
    SFDD new_sfdd;
    new_sfdd.vtree_index = normalized_sfdd1.vtree_index;

    if (normalized_sfdd1.terminal() && normalized_sfdd2.terminal()) {
        // base case
        if (normalized_sfdd1.equals(normalized_sfdd2)) {
            new_sfdd.value = 0;
        } else if (normalized_sfdd1.zero() || normalized_sfdd2.zero()) {
            new_sfdd.value = normalized_sfdd1.value+normalized_sfdd2.value;
        } else if (normalized_sfdd1.one()) {
            new_sfdd.value = normalized_sfdd2.value/2*2+1-normalized_sfdd2.value%2;
        } else if (normalized_sfdd2.one()) {
            new_sfdd.value = normalized_sfdd1.value/2*2+1-normalized_sfdd1.value%2;
        } else {
            new_sfdd.value = 1;
        }
    } else {
        for (vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
        e1 != normalized_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
            e2 != normalized_sfdd2.elements.end(); ++e2) {
                Element new_e;
            cout << "==================================" << endl;
            e1->prime.print();
            cout << "inter with -------------" << endl;
            e2->prime.print();
                new_e.prime = e1->prime.Intersection(e2->prime, m);
            cout << "get --------------------" << endl;
            new_e.prime.print();
            cout << "==================================" << endl;
                if (!new_e.prime.zero()) {
                    new_e.sub = e1->sub.Xor(e2->sub, m);
                    new_sfdd.elements.push_back(new_e);
                }
            }
        }
    }
    return new_sfdd.reduced(m);
}

SFDD SFDD::And(const SFDD & sfdd, const Manager & m) const {
    // cout << "And..." << endl;
    if (empty()) return sfdd;
    if (sfdd.empty()) return *this;

    if (zero()) return *this;
    if (sfdd.zero()) return sfdd;
    if (one()) return sfdd;
    if (sfdd.one()) return *this;

    SFDD new_sfdd;
    new_sfdd.vtree_index = vtree_index;

    SFDD normalized_sfdd1 = *this, normalized_sfdd2 = sfdd;
    if ((!m.vtree->leaf(normalized_sfdd1.vtree_index) || !m.vtree->leaf(normalized_sfdd2.vtree_index)) \
        && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, m.vtree);
        normalized_sfdd1.normalized(lca, m);
        normalized_sfdd2.normalized(lca, m);
    }

    if (normalized_sfdd1.terminal() && normalized_sfdd2.terminal()) {
        // base case
        if (normalized_sfdd1.zero() || normalized_sfdd2.zero()) {
            new_sfdd.value = 0;
        } else if (normalized_sfdd1.one() || normalized_sfdd2.one()) {
            new_sfdd.value = normalized_sfdd1.value*normalized_sfdd2.value;
        } else if (normalized_sfdd1.equals(normalized_sfdd2)) {
            new_sfdd = normalized_sfdd1;
        } else {
            new_sfdd.value = 0;
        }
    } else {
        for (vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
        e1 != normalized_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
            e2 != normalized_sfdd2.elements.end(); ++e2) {
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
        }
    }
    return new_sfdd.reduced(m);
}

SFDD SFDD::Or(const SFDD & sfdd, const Manager & m) const {
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
        if (terminal()) {
            if (zero() || one()) {
                out_dot << "\t" << value << " [shape=record, label=\"" \
                << value << "\"]" << endl << "}";
            } else {
                out_dot << "\t";
                if (negative()) out_dot << "neg";
                out_dot << "x" << value/2 << " [shape=record, label=\"";
                if (negative()) out_dot << "-";
                out_dot << "x" << value/2 << "\"]" << endl << "}";
            }
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

void SFDD::save_file_as_dot(const string f_name) const {
    fstream f;
    f.open("dotG/test0/"+f_name+".dot", fstream::out | fstream::trunc);
    print_dot(f, true);
    f.close();
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

SFDD Manager::sfddZero() const {
    return *(new SFDD(0));
}

SFDD Manager::sfddOne() const {
    return *(new SFDD(1));
}

SFDD Manager::sfddVar(const int tmp_var) {
    assert(tmp_var != 0);
    return *(new SFDD(tmp_var < 0 ? (0-tmp_var)*2+1 : tmp_var*2, get_index_by_var[abs(tmp_var)]));
}

extern SFDD normalization_1(const Vtree* v, const SFDD & rsfdd, const Manager & m) {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // base case
    if (v->var || (!rsfdd.terminal() && v->index == rsfdd.vtree_index)) {
        return rsfdd;
    }

    Element e1, e2;
    if (rsfdd.zero()) {
        // rule 1.(d)
        e1.prime = normalization_2(v->lt, m.sfddZero(), m);
        e1.sub = normalization_1(v->rt, m.sfddZero(), m);
        sfdd.elements.push_back(e1);
        return sfdd;
    } else if (rsfdd.one()) {
        // rule 1.(c)
        e1.prime = normalization_1(v->lt, m.sfddOne(), m);
        e1.sub = normalization_1(v->rt, m.sfddOne(), m);  // normalization
        e2.prime = normalization_2(v->lt, m.sfddOne(), m);
        e2.sub = normalization_1(v->rt, m.sfddZero(), m);
    } else if (rsfdd.vtree_index < v->index) {
        // rule 1.(a)
        e1.prime = normalization_1(v->lt, rsfdd, m);  // normalization
        e1.sub = normalization_1(v->rt, m.sfddOne(), m);
        e2.prime = normalization_2(v->lt, rsfdd, m);
        e2.sub = normalization_1(v->rt, m.sfddZero(), m);
    } else {
        // rule 1.(b)
        e1.prime = normalization_1(v->lt, m.sfddOne(), m);
        e1.sub = normalization_1(v->rt, rsfdd, m);  // normalization
        e2.prime = normalization_2(v->lt, m.sfddOne(), m);
        e2.sub = normalization_1(v->rt, m.sfddZero(), m);
    }
    sfdd.elements.push_back(e1);
    sfdd.elements.push_back(e2);
    return sfdd;
}

/*
 * @para: v - vtree, rsfdd - in compiling rules 2 and 3, there
 *        are formulas like $f \oplus g$, rsfdd means 'g' in
 *        this kind format.
 */
extern SFDD normalization_2(const Vtree* v, const SFDD & rsfdd, const Manager & m) {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // check if constant
    if (v->var) {
        if (rsfdd.one()) sfdd.value = v->var*2;
        else if (rsfdd.zero()) sfdd.value = v->var*2+1;
        else if (rsfdd.value%2 == 0) sfdd.value = 1;
        else sfdd.value = 0;
        return sfdd;
    }
    if (v->index == rsfdd.vtree_index)
        return normalization_2(v, m.sfddZero(), m).Xor(rsfdd, m);

    Element e1, e2;
    if (rsfdd.zero()) {
        // rule 2.(d)
        e1.prime = normalization_2(v->lt, m.sfddZero(), m);
        e1.sub = normalization_2(v->rt, m.sfddZero(), m);
        sfdd.elements.push_back(e1);
        return sfdd;
    } else if (rsfdd.one()) {
        // rule 2.(c)
        e1.prime = normalization_1(v->lt, m.sfddOne(), m);
        e1.sub = normalization_2(v->rt, m.sfddOne(), m);  // normalization
        e2.prime = normalization_2(v->lt, m.sfddOne(), m);
        e2.sub = normalization_2(v->rt, m.sfddZero(), m);
    } else if (rsfdd.vtree_index < v->index) {
        // rule 2.(a)
        e1.prime = normalization_1(v->lt, rsfdd, m);  // normalization
        e1.sub = normalization_2(v->rt, m.sfddOne(), m);
        e2.prime = normalization_2(v->lt, rsfdd, m);
        e2.sub = normalization_2(v->rt, m.sfddZero(), m);
    } else {
        // rule 2.(b)
        e1.prime = normalization_1(v->lt, m.sfddOne(), m);
        e1.sub = normalization_2(v->rt, rsfdd, m);  // normalization
        e2.prime = normalization_2(v->lt, m.sfddOne(), m);
        e2.sub = normalization_2(v->rt, m.sfddZero(), m);
    }
    sfdd.elements.push_back(e1);
    sfdd.elements.push_back(e2);
    return sfdd;
}
