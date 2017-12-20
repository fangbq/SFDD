#include "SFDD.h"


map<int, int> get_index_by_var = {{0,0}};  // x1 stored as "1", x4 stored as "4"...
                                           // indexs of true and false both are "0"


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

Vtree::Vtree(int start_var_index, int end_var_index, vector<int> full_order, VTREE_TYPE t) {
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
            // cout << mid << endl;
            lt = new Vtree(start_var_index, mid-1, full_order, t);
            rt = new Vtree(mid+1, end_var_index, full_order, t);
            size = lt->size + rt->size + 1;
            break;
        }
        default:
            cout << "vtree error 2" << endl;
            ;
    }
    return;
}

Vtree::Vtree(const Vtree& v)
    : index(v.index), var(v.var), lt(NULL), rt(NULL), size(v.size) {
    if (v.lt) lt = new Vtree(*v.lt);
    if (v.rt) rt = new Vtree(*v.rt);
}

Vtree::Vtree(const string& file_name) {
    ifstream ifs(file_name, ios::in);
    string line;

    while (getline(ifs, line)) {
        if (line[0] == 'c') continue;
        size = stoi(line.substr(6, line.length()-6));
        break;
    }

    // reading vtree nodes from file
    char node_type = ' ';
    int left_child, right_child, parent, var;
    stack<Vtree> vtree_nodes;
    for (int i = 0; i < size; ++i) {
        ifs >> node_type;
        if (node_type == 'L') {
            ifs >> parent >> var;
            vtree_nodes.push(*new Vtree(++parent, var));
        } else if (node_type == 'I') {
            ifs >> parent >> left_child >> right_child;
            vtree_nodes.push(*new Vtree(++parent, ++left_child, ++right_child));
        } else {
            cerr << "reading vtree file, node type input error" << endl;
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
Vtree& Vtree::merge(const Vtree& hat) {
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
        cerr << "Vtree merge error!!!" << endl;
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
        cout << "father_of error!" << endl;
        exit(1);
    }
}

bool Vtree::is_leaf(int i) const {
    if (i == 0) return true;  // value is true or false when vtree_index equals zero
    return subvtree(i).lt == NULL;
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
    // cout << this << endl;
    if (lt) lt->print(indent+1);
    if (rt) rt->print(indent+1);
    return;
}

void Vtree::print_dot(fstream& out_dot, bool root) const {
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

void Vtree::print_vtree(fstream& out_dot, bool root) const {
    if (root) out_dot << "vtree " << size << endl;
    if (rt) rt->print_vtree(out_dot, false);
    if (lt) lt->print_vtree(out_dot, false);
    if (var) out_dot << "L " << index << " " << var << endl;
    else out_dot << "I " << index << " " << lt->index << " " << rt->index << endl;
}

void Vtree::save_dot_file(const string f_name) const {
    fstream f;
    f.open(f_name, fstream::out | fstream::trunc);
    print_dot(f, true);
    f.close();
}

void Vtree::save_vtree_file(const string f_name) const {
    fstream f;
    f.open(f_name, fstream::out | fstream::trunc);
    print_vtree(f, true);
    f.close();
}

int SFDD::size() const {
    if (is_empty()) return 0;
    if (is_terminal()) return 1;

    int size = elements.size();
    for (vector<Element>::const_iterator e = elements.begin(); \
    e != elements.end(); ++e) {
        size += e->prime.size();
        size += e->sub.size();
    }
    return size;
}

bool SFDD::computable_with(const SFDD& sfdd, const Manager& m) const {
    return is_terminal() && sfdd.is_terminal() && (is_constant() || sfdd.is_constant() || \
        // m.vtree->father_of(vtree_index) == m.vtree->father_of(sfdd.vtree_index));
        vtree_index == sfdd.vtree_index);
}

bool SFDD::operator==(const SFDD& sfdd) const {
// cout << "equals..." << endl;
    if (is_terminal() && sfdd.is_terminal()) {
        return value == sfdd.value;
    } else if (elements.size() == sfdd.elements.size()) {
        set<int> equaled_elements;
        for (vector<Element>::const_iterator e1 = elements.begin(); \
        e1 != elements.end(); ++e1) {
            bool found_equivalent = false;
            for (int i = 0; i < sfdd.elements.size(); ++i) {
                if (equaled_elements.find(i) == equaled_elements.end()
                && e1->equals(sfdd.elements[i])) {
                    equaled_elements.insert(i);
                    found_equivalent = true;
                    break;
                }
            }
            if (!found_equivalent) {
                // cout << "no eq" << endl;
                return false;
            }
        }
        // cout << "no eq" << endl;
        return true;
    }
    // cout << "no eq" << endl;
    return false;
}

SFDD& SFDD::operator=(const SFDD& sfdd) {
    value = sfdd.value;
    elements = sfdd.elements;
    vtree_index = sfdd.vtree_index;
    return *this;
}

SFDD& SFDD::reduced(Manager& m) {
// cout << "reduced..." << endl;
    if (is_terminal()) return *this;
    bool valid = false;
    for (vector<Element>::iterator e = elements.begin(); \
    e != elements.end(); ) {
        // 1.1 removes those elements that primes are false
        if (e->prime.reduced(m).is_zero()) {
            e = elements.erase(e);
        } else if (!e->sub.reduced(m).is_zero()) {
            valid = true;
            ++e;
        } else {
            ++e;
        }
    }
    // 1.2 return false if all elements' subs are false
    if (!valid) elements.clear();
    if (size()==0) value = 0;
    //3 trimming
// cout << "trimming..." << endl;
    if (elements.size() == 1) {
        // {(-a, T)} -> -a
        if (elements[0].prime.is_terminal() && elements[0].sub.is_one()) {
            value = elements[0].prime.value;
            elements.clear();
        }
    }
    if (elements.size() == 2) {
        // {(a, 1), (other-pi-terms/a, 0)} -> a
        // {(1, a), (other-pi-terms/1, 0)} -> a
        if (elements[0].sub.is_zero()) {
            if (elements[1].sub.is_one() && elements[1].prime.is_terminal()) {
                value = elements[1].prime.value;
                elements.clear();
            } else if (elements[1].prime.is_one() && elements[1].sub.is_terminal()) {
                value = elements[1].sub.value;
                elements.clear();
            }
        } else if (elements[1].sub.is_zero()) {
            if (elements[0].sub.is_one() && elements[0].prime.is_terminal()) {
                value = elements[0].prime.value;
                elements.clear();
            } else if (elements[0].prime.is_one() && elements[0].sub.is_terminal()) {
                value = elements[0].sub.value;
                elements.clear();
            }
        }
    }
    // 2 compressing (necessary)
    for (vector<Element>::iterator e1 = elements.begin(); \
    e1 != elements.end(); ) {
        bool is_delete = false;
        for (vector<Element>::iterator e2 = elements.begin(); \
        e2 != elements.end(); ++e2) {
            // cout << "big equal..." << endl;
            if (e1 != e2 && e1->sub==e2->sub) {
                is_delete = true;
                e2->prime = e2->prime.Xor(e1->prime, m).reduced(m);
                e1 = elements.erase(e1);
                break;
            }
        }
        if (!is_delete)
            ++e1;
    }

    if (is_terminal()) vtree_index = get_index_by_var[value/2];
    return *this;
}

extern int get_lca(int a, int b, const Vtree& v) {
// cout << "get_lca..." << endl;
    // if (!&v) return 0;
    // cout << a << " " << v.index << " " << b << endl;
    if (v.index == a || v.index == b) return v.index;
    int L = v.lt ? get_lca(a, b, *v.lt) : 0;
    int R = v.rt ? get_lca(a, b, *v.rt) : 0;
    if (L && R) return v.index;  // if p and q are on both sides
    return L ? L : R;  // either one of p,q is on one side OR p,q is not in L&R subtrees
}

SFDD& SFDD::normalized(int lca, Manager& m) {
// cout << "normalized..." << endl;
    for (vector<Element>::iterator e = elements.begin(); \
    e != elements.end(); ++e) {
        e->prime.normalized(m.vtree->subvtree(vtree_index).lt->index, m);
        e->sub.normalized(m.vtree->subvtree(vtree_index).rt->index, m);
    }
    *this = normalization_1(m.vtree->subvtree(lca), *this, m);
    if (!is_terminal()) value = -1;
    return *this;
}

SFDD SFDD::Intersection(const SFDD& sfdd, Manager& m, int print_info) const {
// cout << "Intersection..." << endl;    
    addr_t this_id = m.make_or_find(*this), sfdd_id = m.make_or_find(sfdd);
    addr_t cache = m.read_cache(INTER, this_id, sfdd_id);
    if (cache != SFDD_NULL)
        return m.sfdd_nodes_[cache];

    if (is_zero()) return *this;
    if (sfdd.is_zero()) return sfdd;

    SFDD new_sfdd;
    new_sfdd.vtree_index = vtree_index;
    // normalizing for both sides
    SFDD normalized_sfdd1 = *this, normalized_sfdd2 = sfdd;
    if (!computable_with(sfdd, m) && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, *m.vtree);
        normalized_sfdd1.normalized(lca, m);
        normalized_sfdd2.normalized(lca, m);
        new_sfdd.vtree_index = lca;
    }

    if (normalized_sfdd1.is_terminal() && normalized_sfdd2.is_terminal()) {
        // base case
        if (normalized_sfdd1==normalized_sfdd2 || normalized_sfdd2.is_negative())
            new_sfdd = normalized_sfdd1;
        else if (normalized_sfdd1.is_negative())
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
                if (!new_e.prime.is_zero()) {
                    new_e.sub = e1->sub.Intersection(e2->sub, m);
                    new_sfdd.elements.push_back(new_e);
                }
            }
        }
    }
    new_sfdd.reduced(m);
    if (new_sfdd.is_terminal()) new_sfdd.vtree_index = get_index_by_var[new_sfdd.value/2];
    
    addr_t new_sfdd_id = m.make_sfdd(new_sfdd);
    m.write_cache(INTER, this_id, sfdd_id, new_sfdd_id);
    m.write_cache(INTER, sfdd_id, this_id, new_sfdd_id);
    return new_sfdd;
}

SFDD SFDD::Xor(const SFDD& sfdd, Manager& m, bool do_nml) const {
// cout << "Xor..." << endl;
    addr_t this_id = m.make_or_find(*this), sfdd_id = m.make_or_find(sfdd);
    addr_t cache = m.read_cache(XOR, this_id, sfdd_id);
    if (cache != SFDD_NULL)
        return m.sfdd_nodes_[cache];

    if (is_zero()) return sfdd;
    if (sfdd.is_zero()) return *this;

    SFDD new_sfdd;
    new_sfdd.vtree_index = vtree_index;
    SFDD normalized_sfdd1 = *this, normalized_sfdd2 = sfdd;
    if (do_nml || (!computable_with(sfdd, m) && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index)) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, *m.vtree);
        normalized_sfdd1.normalized(lca, m);
        normalized_sfdd2.normalized(lca, m);
        new_sfdd.vtree_index = lca;
    }

    if (normalized_sfdd1.is_terminal() && normalized_sfdd2.is_terminal()) {
        // base case
        if (normalized_sfdd1==normalized_sfdd2) {
            new_sfdd.value = 0;
        } else if (normalized_sfdd1.is_zero() || normalized_sfdd2.is_zero()) {
            new_sfdd.value = normalized_sfdd1.value+normalized_sfdd2.value;
        } else if (normalized_sfdd1.is_one()) {
            new_sfdd.value = normalized_sfdd2.value/2*2+1-normalized_sfdd2.value%2;
        } else if (normalized_sfdd2.is_one()) {
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
                new_e.prime = e1->prime.Intersection(e2->prime, m, do_nml);
                if (!new_e.prime.is_zero()) {
                    new_e.sub = e1->sub.Xor(e2->sub, m);
                    new_sfdd.elements.push_back(new_e);
                }
            }
        }
    }
    new_sfdd.reduced(m);
    if (new_sfdd.is_terminal()) new_sfdd.vtree_index = get_index_by_var[new_sfdd.value/2];

    addr_t new_sfdd_id = m.make_sfdd(new_sfdd);
    m.write_cache(XOR, this_id, sfdd_id, new_sfdd_id);
    m.write_cache(XOR, sfdd_id, this_id, new_sfdd_id);
    return new_sfdd;
}

SFDD SFDD::And(const SFDD& sfdd, Manager& m, bool do_nml, int clause_counter) const {
// cout << "And..." << endl;
    addr_t this_id = m.make_or_find(*this), sfdd_id = m.make_or_find(sfdd);
    addr_t cache = m.read_cache(AND, this_id, sfdd_id);
    if (cache != SFDD_NULL)
        return m.sfdd_nodes_[cache];

    if (is_zero()) return *this;
    if (sfdd.is_zero()) return sfdd;
    if (is_one()) return sfdd;
    if (sfdd.is_one()) return *this;

    SFDD new_sfdd;
    new_sfdd.vtree_index = vtree_index;
    SFDD normalized_sfdd1 = *this, normalized_sfdd2 = sfdd;
    if (do_nml || (!computable_with(sfdd, m) && normalized_sfdd1.vtree_index != normalized_sfdd2.vtree_index)) {
        int lca = get_lca(normalized_sfdd1.vtree_index, normalized_sfdd2.vtree_index, *m.vtree);
        normalized_sfdd1.normalized(lca, m);
        normalized_sfdd2.normalized(lca, m);
        new_sfdd.vtree_index = lca;
    }
    if (normalized_sfdd1.is_terminal() && normalized_sfdd2.is_terminal()) {
        // base case
        if (normalized_sfdd1.is_zero() || normalized_sfdd2.is_zero()) {
            new_sfdd.value = 0;
        } else if (normalized_sfdd1.is_one() || normalized_sfdd2.is_one()) {
            new_sfdd.value = normalized_sfdd1.value*normalized_sfdd2.value;
        } else if (normalized_sfdd1==normalized_sfdd2) {
            new_sfdd = normalized_sfdd1;
        } else {
            new_sfdd.value = 0;
        }
    } else {
        for (vector<Element>::const_iterator e1 = normalized_sfdd1.elements.begin();
        e1 != normalized_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = normalized_sfdd2.elements.begin();
            e2 != normalized_sfdd2.elements.end(); ++e2) {
                SFDD prime_product = e1->prime.And(e2->prime, m);
                if (prime_product.is_zero())
                    continue;
                SFDD sub_product = e1->sub.And(e2->sub, m);
                SFDD tmp_new_sfdd;
                tmp_new_sfdd.vtree_index = new_sfdd.vtree_index;
                for (vector<Element>::const_iterator new_e = new_sfdd.elements.begin();
                new_e != new_sfdd.elements.end(); ++new_e) {
                    Element inter_e;
                    inter_e.prime = new_e->prime.Intersection(prime_product, m);
                    if (!inter_e.prime.is_zero()) {
                        Element origin_e;
                        inter_e.sub = new_e->sub.Xor(sub_product, m);
                        tmp_new_sfdd.elements.push_back(inter_e);  // add inter-ele
                        origin_e.prime = new_e->prime.Xor(inter_e.prime, m);
                        origin_e.sub = new_e->sub;
                        tmp_new_sfdd.elements.push_back(origin_e);  // add orig-ele
                        prime_product = prime_product.Xor(inter_e.prime, m);
                    } else {
                        tmp_new_sfdd.elements.push_back(*new_e);
                    }
                }
                if (!prime_product.is_zero()) {
                    Element last_e;  // last element that has removed all common parts with others
                    last_e.prime = prime_product;
                    last_e.sub = sub_product;
                    tmp_new_sfdd.elements.push_back(last_e);  // add last element
                }
                new_sfdd = tmp_new_sfdd;
            }
        }
    }
    new_sfdd.reduced(m);
    if (new_sfdd.is_terminal()) new_sfdd.vtree_index = get_index_by_var[new_sfdd.value/2];

    addr_t new_sfdd_id = m.make_sfdd(new_sfdd);
    m.write_cache(AND, this_id, sfdd_id, new_sfdd_id);
    m.write_cache(AND, sfdd_id, this_id, new_sfdd_id);
    return new_sfdd;
}

SFDD SFDD::Or(const SFDD& sfdd, Manager& m, bool do_nml) const {
// cout << "Or..." << endl;
    addr_t this_id = m.make_or_find(*this), sfdd_id = m.make_or_find(sfdd);
    addr_t cache = m.read_cache(OR, this_id, sfdd_id);
    if (cache != SFDD_NULL)
        return m.sfdd_nodes_[cache];

    if (is_empty()) return sfdd;
    if (sfdd.is_empty()) return *this;

    SFDD new_sfdd = Xor(sfdd, m, true).Xor(And(sfdd, m, true), m, true);
    addr_t new_sfdd_id = m.make_sfdd(new_sfdd);
    m.write_cache(OR, this_id, sfdd_id, new_sfdd_id);
    m.write_cache(OR, sfdd_id, this_id, new_sfdd_id);
    return new_sfdd;
}

SFDD SFDD::Not(Manager& m) const {
    return Xor(m.sfddOne(), m);
}

void SFDD::print(int indent) const {
    if (elements.empty()) {
        for (int i = 0; i < indent; ++i) cout << " ";
        if (value < 2) {
            // cout << value << endl;
            cout << value << " ~ " << vtree_index << endl;
        } else {
            if (is_negative()) cout << "-";
            // cout << "x" << value/2 << endl;
            cout << "x" << value/2 << " ~ " << vtree_index << endl;
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

void SFDD::print_dot(fstream& out_dot, bool root, int depth, string dec_name) const {
    if (root) {
        out_dot << "digraph G {" << endl;
        if (is_terminal()) {
            if (is_constant()) {
                out_dot << "\t" << value << " [shape=record, label=\"" \
                << value << "\"]" << endl << "}";
                // << value << " ~ " << vtree_index << "\"]" << endl << "}";
            } else {
                out_dot << "\t";
                if (is_negative()) out_dot << "neg";
                out_dot << "x" << value/2 << " [shape=record, label=\"";
                if (is_negative()) out_dot << "-";
                out_dot << "x" << value/2 << "\"]" << endl << "}";
                // out_dot << "x" << value/2 << " ~ " << vtree_index << "\"]" << endl << "}";
            }
            return;
        }
    }
    if (elements.empty()) {
        if (value < 2) {
            out_dot << value;
            // out_dot << value << "~" << vtree_index;
        } else {
            if (is_negative()) out_dot << "-";
            out_dot << "x" << value/2;
            // out_dot << "x" << value/2 << "~" << vtree_index;
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
    f.open(f_name, fstream::out | fstream::trunc);
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

void Element::print_dot(fstream& out_dot, int depth, string e_name) const {
    out_dot << "\t" << e_name << " [shape=record,label=\"<f0> ";
    bool prime_out_edge = false;
    bool sub_out_edge = false;
    if (prime.is_terminal()) prime.print_dot(out_dot);
    else { out_dot << "●"; prime_out_edge = true; }
    out_dot << "|<f1> ";
    if (sub.is_terminal()) sub.print_dot(out_dot);
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

Manager::Manager(const Vtree& v) : cache_table_(INIT_SIZE) {
    vtree = new Vtree(v);
};

Manager::~Manager() {
    if (vtree != NULL) { delete vtree; vtree = NULL; }
};

SFDD Manager::sfddZero() {
    SFDD sfdd(0);
    size_t node_id = make_or_find(sfdd);
    return sfdd_nodes_[node_id]; 
}
SFDD Manager::sfddOne() {
    SFDD sfdd(1);
    size_t node_id = make_or_find(sfdd);
    return sfdd_nodes_[node_id]; 
}

SFDD Manager::sfddVar(const int tmp_var) {
    assert(tmp_var != 0);
    SFDD sfdd(tmp_var < 0 ? (0-tmp_var)*2+1 : tmp_var*2, get_index_by_var[abs(tmp_var)]);
    size_t node_id = make_or_find(sfdd);
    // cout << node_id << endl;
    return sfdd_nodes_[node_id];
    // return sfdd;
}

addr_t Manager::make_sfdd(const SFDD& new_sfdd) {
    sfdd_nodes_.emplace_back();
    size_t node_id = sfdd_nodes_.size()-1;
    uniq_table_.emplace(new_sfdd, node_id);
    sfdd_nodes_[node_id] = new_sfdd;
    return node_id;
}

addr_t Manager::make_or_find(const SFDD& new_sfdd) {
    auto res = uniq_table_.find(new_sfdd);
    if (res != uniq_table_.end()) {
        return res->second;
    }
    return make_sfdd(new_sfdd);
}

void Manager::write_cache(const OPERATOR_TYPE op, const addr_t lhs, 
                 const addr_t rhs, const addr_t res) {
    auto key = calc_key(op, lhs, rhs);
    cache_table_[key] = std::make_tuple(op, lhs, rhs, res);
}

void Manager::clear_cache() {
    for (auto it =  cache_table_.begin(); it != cache_table_.end(); ++it) {
        *it = std::make_tuple(OPERATOR_TYPE::NULLOP, -1, -1, -1);
    }
}

addr_t Manager::read_cache(const OPERATOR_TYPE op, const addr_t lhs, const addr_t rhs) {
    auto key = calc_key(op, lhs, rhs);
    auto res = cache_table_[key];
    
    if (std::get<0>(res) == op &&
        std::get<1>(res) == lhs &&
        std::get<2>(res) == rhs) {
        // cout << "got one: " << op << " " << lhs << " " << rhs << " " << get<3>(res) << endl;
        return std::get<3>(res);
    }
    return SFDD_NULL;
}

size_t Manager::calc_key(const OPERATOR_TYPE op, const addr_t lhs,  const addr_t rhs) {
    size_t key = 0;
    hash_combine(key, std::hash<int>()(static_cast<int>(op)));
    hash_combine(key, std::hash<size_t>()(lhs));
    hash_combine(key, std::hash<size_t>()(rhs));
    // cout << "cache table size: " << cache_table_.size() << endl;
    return key % cache_table_.size();
}

extern SFDD normalization_1(const Vtree& v, const SFDD& rsfdd, Manager& m) {
    SFDD sfdd;
    sfdd.vtree_index = v.index;
    // base case
    if (v.var || (!rsfdd.is_terminal() && v.index == rsfdd.vtree_index)) {
        sfdd = rsfdd;
        sfdd.vtree_index = v.index;
        return sfdd;
    }

    Element e1, e2;
    if (rsfdd.is_zero()) {
        // rule 1.(d)
        e1.prime = normalization_2(*v.lt, m.sfddZero(), m);
        e1.sub = normalization_1(*v.rt, m.sfddZero(), m);
        sfdd.elements.push_back(e1);
        return sfdd;
    } else if (rsfdd.is_one()) {
        // rule 1.(c)
        e1.prime = normalization_1(*v.lt, m.sfddOne(), m);
        e1.sub = normalization_1(*v.rt, m.sfddOne(), m);  // normalization
        e2.prime = normalization_2(*v.lt, m.sfddOne(), m);
        e2.sub = normalization_1(*v.rt, m.sfddZero(), m);
    } else if (rsfdd.vtree_index < v.index) {
        // rule 1.(a)
        e1.prime = normalization_1(*v.lt, rsfdd, m);  // normalization
        e1.sub = normalization_1(*v.rt, m.sfddOne(), m);
        e2.prime = normalization_2(*v.lt, rsfdd, m);
        e2.sub = normalization_1(*v.rt, m.sfddZero(), m);
    } else {
        // rule 1.(b)
        e1.prime = normalization_1(*v.lt, m.sfddOne(), m);
        e1.sub = normalization_1(*v.rt, rsfdd, m);  // normalization
        e2.prime = normalization_2(*v.lt, m.sfddOne(), m);
        e2.sub = normalization_1(*v.rt, m.sfddZero(), m);
    }
    if (!e1.prime.is_zero()) sfdd.elements.push_back(e1);
    if (!e2.prime.is_zero()) sfdd.elements.push_back(e2);
    m.make_or_find(sfdd);
    return sfdd;
}

/*
 * @para: v - vtree, rsfdd - in compiling rules 2 and 3, there
 *        are formulas like $f \oplus g$, rsfdd means 'g' in
 *        this kind format.
 */
extern SFDD normalization_2(const Vtree& v, const SFDD& rsfdd, Manager& m) {
    SFDD sfdd;
    sfdd.vtree_index = v.index;
    // check if constant
    if (v.var) {
        if (rsfdd.is_one()) sfdd.value = v.var*2;
        else if (rsfdd.is_zero()) sfdd.value = v.var*2+1;
        else if (rsfdd.value%2 == 0) sfdd.value = 1;
        else sfdd.value = 0;
        return sfdd;
    }
    if (v.index == rsfdd.vtree_index)
        return normalization_2(v, m.sfddZero(), m).Xor(rsfdd, m);

    Element e1, e2;
    if (rsfdd.is_zero()) {
        // rule 2.(d)
        e1.prime = normalization_2(*v.lt, m.sfddZero(), m);
        e1.sub = normalization_2(*v.rt, m.sfddZero(), m);
        sfdd.elements.push_back(e1);
        return sfdd;
    } else if (rsfdd.is_one()) {
        // rule 2.(c)
        e1.prime = normalization_1(*v.lt, m.sfddOne(), m);
        e1.sub = normalization_2(*v.rt, m.sfddOne(), m);  // normalization
        e2.prime = normalization_2(*v.lt, m.sfddOne(), m);
        e2.sub = normalization_2(*v.rt, m.sfddZero(), m);
    } else if (rsfdd.vtree_index < v.index) {
        // rule 2.(a)
        e1.prime = normalization_1(*v.lt, rsfdd, m);  // normalization
        e1.sub = normalization_2(*v.rt, m.sfddOne(), m);
        e2.prime = normalization_2(*v.lt, rsfdd, m);
        e2.sub = normalization_2(*v.rt, m.sfddZero(), m);
    } else {
        // rule 2.(b)
        e1.prime = normalization_1(*v.lt, m.sfddOne(), m);
        e1.sub = normalization_2(*v.rt, rsfdd, m);  // normalization
        e2.prime = normalization_2(*v.lt, m.sfddOne(), m);
        e2.sub = normalization_2(*v.rt, m.sfddZero(), m);
    }
    if (!e1.prime.is_zero()) sfdd.elements.push_back(e1);
    if (!e2.prime.is_zero()) sfdd.elements.push_back(e2);
    m.make_or_find(sfdd);
    return sfdd;
}
