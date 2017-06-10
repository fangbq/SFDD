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

bool SFDD::equals(const SFDD & sfdd) const {
    if (terminal() && sfdd.terminal()) {
        return (constant ? constant==sfdd.constant : lit==sfdd.lit);
    } else if (size() == sfdd.size()) {
        set<int> equaled_elements;
        for (vector<Element>::const_iterator e1 = elements.begin(); \
        e1 != elements.end(); ++e1) {
            bool found_equivalent = false;
            for (int i = 0; i < sfdd.elements.size(); ++i) {
                if (equaled_elements.find(i) != equaled_elements.end()
                && e1->equals(sfdd.elements[i])) {
                    equaled_elements.insert(i);
                    found_equivalent = true;
                    break;
                }
            }
            if (!found_equivalent) return false;
        }
    }
    return false;
}

SFDD& SFDD::reduced(Manager & m) {
    if (terminal()) return *this;
    bool valid = false;
    for (vector<Element>::iterator e = elements.begin(); \
    e != elements.end(); ++e) {
        // 1.1 removes those elements that primes are false
        if (e->prime.reduced(m).zero())
            elements.erase(e);
        else if (!e->sub.reduced(m).zero())
            valid = true;
    }
    // 1.2 return false if all elements' subs are false
    if (!valid) elements.clear();
    if (size()==0) constant = -1;
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
    if (constant) {
        // return {(1, 1), (x, 0)} if 1; return {(1, 0), (x, 0)} if 0
        SFDD expanded_sfdd;
        expanded_sfdd.vtree_index = vtree_index;
        Element e1, e2;
        e1.prime.constant = 1;
        e1.sub.constant = constant;
        int leftest_lit = 0;
        Vtree* tmp_v = m.vtree;
        while (tmp_v) {
            leftest_lit = tmp_v->var;
            tmp_v = tmp_v->lt;
        }
        e2.prime.lit = leftest_lit;
        e2.sub.constant = -1;
        expanded_sfdd.elements.push_back(e1);
        expanded_sfdd.elements.push_back(e2);
        return expanded_sfdd;
    }
    // return self if non-constant
    return *this;
}

SFDD SFDD::Intersection(const SFDD & sfdd, Manager & m) const {
    SFDD new_sfdd;
    new_sfdd.vtree_index = vtree_index;
    if (terminal() && sfdd.terminal()) {
        // base case
        if (equals(sfdd) || sfdd.lit < 0)
            new_sfdd = *this;
        else if (lit < 0)
            new_sfdd = sfdd;
        else
            new_sfdd.constant = -1;
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
        SFDD outer_sfdd, inner_sfdd;
        if (vtree_index > sfdd.vtree_index) {
            outer_sfdd = expanded(m);
            inner_sfdd = sfdd.expanded(m);
        } else {
            outer_sfdd = sfdd.expanded(m);
            inner_sfdd = expanded(m);
        }
        for (vector<Element>::const_iterator outer_e = outer_sfdd.elements.begin();
        outer_e != outer_sfdd.elements.end(); ++outer_e) {
            Element new_e;
            new_e.prime = outer_e->prime.Intersection(inner_sfdd, m).reduced(m);
            new_e.sub = outer_e->sub;
            new_sfdd.elements.push_back(new_e);
        }
    }
    return new_sfdd.reduced(m);
}

SFDD SFDD::Xor(const SFDD & sfdd, Manager & m) const {
    SFDD new_sfdd;
    if (terminal() && sfdd.terminal()) {
        // base case
        if (equals(sfdd)) {
            new_sfdd.constant = -1;
        } else if (zero() || sfdd.zero()) {
            new_sfdd.constant = max(constant, sfdd.constant);
            new_sfdd.lit = lit+sfdd.lit;
        } else if (one() || sfdd.one()) {
            new_sfdd.constant = min(constant, sfdd.constant);
            new_sfdd.lit = -(lit+sfdd.lit);
        } else {
            new_sfdd.constant = 1;
            new_sfdd.lit = 0;
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
        SFDD outer_sfdd, inner_sfdd;
        if (vtree_index > sfdd.vtree_index) {
            outer_sfdd = expanded(m);
            inner_sfdd = sfdd.expanded(m);
        } else {
            outer_sfdd = sfdd.expanded(m);
            inner_sfdd = expanded(m);
        }
        new_sfdd.vtree_index = outer_sfdd.vtree_index;
        for (vector<Element>::const_iterator outer_e = outer_sfdd.elements.begin();
        outer_e != outer_sfdd.elements.end(); ++outer_e) {
            Element new_e;
            new_e.prime = outer_e->prime.Xor(inner_sfdd, m).reduced(m);
            new_e.sub = outer_e->sub;
            new_sfdd.elements.push_back(new_e);
        }
    }
    return new_sfdd.reduced(m);
}

SFDD SFDD::And(const SFDD & sfdd, Manager & m) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::Or(const SFDD & sfdd, Manager & m) const {
    SFDD new_sfdd;
    return new_sfdd;
}

void SFDD::print(int indent) const {
    if (elements.empty()) {
        for (int i = 0; i < indent; ++i) cout << " ";
        if (constant) {
            cout << (constant>0 ? 1 : 0) << endl;
        } else {
            if (lit < 0) cout << "-";
            cout << "x" << abs(lit) << endl;
        }
        return;
    }
    for (int i = 0; i < indent; ++i) cout << " ";
    cout << "Dec " << vtree_index << ":" << endl;
    int counter = 1;
    for (vector<Element>::const_iterator e = elements.begin();
    e != elements.end(); ++e) {
        for (int i = 0; i < indent+1; ++i) cout << " ";
        cout << "E" << counter << "p:" << endl;
        for (int i = 0; i < indent+1; ++i) cout << " ";
        e->prime.print(indent+1);
        for (int i = 0; i < indent+1; ++i) cout << " ";
        cout << "E" << counter++ << "s:" << endl;
        for (int i = 0; i < indent+1; ++i) cout << " ";
        e->sub.print(indent+1);
    }
    return;
}

SFDD Manager::sfddZero() const {
    SFDD sfdd;
    sfdd.vtree_index = 1;
    sfdd.constant = -1;
    return sfdd;
}

SFDD Manager::sfddOne() const {
    SFDD sfdd;
    sfdd.vtree_index = 1;
    sfdd.constant = 1;
    return sfdd;
}

SFDD Manager::sfddVar(const Vtree* v, const int var) const {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // check if constant
    if (v->var) {
        sfdd.lit = var;  /* different with get_SFDD, sfddVar \
                            solves 'f=x_i', so the var must be \
                            the literal in the leaf. */
        return sfdd;
    }
    Element e1, e2;
    set<int> lt_vars = v->lt->get_variables();
    if (lt_vars.find(var) != lt_vars.end()) {
        // rule 1.(a)
        // e1.prime.lit = var;
        e1.prime = sfddVar(v->lt, var);  // normalization
        e1.sub.constant = 1;
        e2.prime = get_SFDD(v->lt, var);
        e2.sub.constant = -1;
    } else {
        // rule 1.(b)
        e1.prime.constant = 1;
        // e1.sub.lit = var;
        e1.sub = sfddVar(v->rt, var);  // normalization
        e2.prime = get_SFDD(v->lt, 0);
        e2.sub.constant = -1;
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
SFDD Manager::get_SFDD(const Vtree* v, const int rht) const {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // check if constant
    if (v->var) {
        sfdd.constant = (rht>0);
        sfdd.lit = (rht==0)*v->var-(rht==-1)*v->var;
        return sfdd;
    }
    if (rht == -1) {
        // rule 4
        Element e;
        e.prime = get_SFDD(v->lt);
        e.sub = get_SFDD(v->rt);
        sfdd.elements.push_back(e);
    } else if (rht == 0) {
        // rule 3
        Element e1, e2;
        e1.prime.constant = 1;
        e1.sub = get_SFDD(v->rt, 0);
        e2.prime = e1.sub;
        e2.sub = get_SFDD(v->rt);
        sfdd.elements.push_back(e1);
        sfdd.elements.push_back(e2);
    } else {
        // rule 2
        Element e1, e2;
        set<int> lt_vars = v->lt->get_variables();
        if (lt_vars.find(rht) != lt_vars.end()) {
            // rule 2.(a)
            // e1.prime.lit = rht;
            e1.prime = sfddVar(v->lt, rht);  // normalization
            e1.sub = get_SFDD(v->rt, 0);
            e2.prime = get_SFDD(v->lt, rht);
            e2.sub = get_SFDD(v->rt);
        } else {
            // rule 2.(b)
            e1.prime.constant = 1;
            e1.sub = get_SFDD(v->rt, rht);
            e2.prime = get_SFDD(v->lt, 0);
            e2.sub = get_SFDD(v->rt);
        }
        sfdd.elements.push_back(e1);
        sfdd.elements.push_back(e2);
    }
    return sfdd;
}
