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

bool SFDD::equal(const SFDD & sfdd) const {
    if (terminal() && sfdd.terminal()) {
        return (constant ? constant==sfdd.constant : lit==sfdd.lit);
    }
    return false;
}

SFDD& SFDD::reduced() {
    return *this;
}

SFDD SFDD::expanded() const {
    if (terminal()) {
        // return {(x, 1)} if x; return ?? if 1; return ?? if 0
        SFDD expanded_sfdd;
        Element e;
        e.prime = *this;
        e.sub.constant = 1;
        expanded_sfdd.elements.push_back(e);
        return expanded_sfdd;
    }
    // return self if non-constant
    return *this;
}

SFDD SFDD::Intersection(const SFDD & sfdd) const {
    SFDD new_sfdd;
    if (terminal() && sfdd.terminal()) {
        // base case
        if (equal(sfdd) || sfdd.lit < 0)
            new_sfdd = *this;
        else if (lit < 0)
            new_sfdd = sfdd;
        else
            new_sfdd.constant = -1;
        // else do nothing and new_sfdd is empty
    } else {
        SFDD expanded_sfdd1 = expanded();
        SFDD expanded_sfdd2 = sfdd.expanded();
        for (vector<Element>::const_iterator e1 = expanded_sfdd1.elements.begin();
        e1 != expanded_sfdd1.elements.end(); ++e1) {
            for (vector<Element>::const_iterator e2 = expanded_sfdd2.elements.begin();
            e2 != expanded_sfdd2.elements.end(); ++e2) {
                Element new_e;
                new_e.prime = e1->prime.Intersection(e2->prime);
                new_e.sub = e1->sub.Intersection(e2->sub);
                if (!new_e.prime.unsatisfiable())
                    new_sfdd.elements.push_back(new_e);
            }
        }
    } 
    return new_sfdd;
}

SFDD SFDD::Xor(const SFDD & sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::And(const SFDD & sfdd) const {
    SFDD new_sfdd;
    return new_sfdd;
}

SFDD SFDD::Or(const SFDD & sfdd) const {
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

SFDD Manager::get_SFDD(const Vtree* v, const int var) const {
    SFDD sfdd;
    sfdd.vtree_index = v->index;
    // check if constant
    if (v->var) {
        sfdd.lit = var;  // different with get_SFDD2, get_SFDD \
                            solves 'f=x_i', so the var must be \
                            the literal in the leaf.
        return sfdd;
    }
    Element e1, e2;
    set<int> lt_vars = v->lt->get_variables();
    if (lt_vars.find(var) != lt_vars.end()) {
        // rule 1.(a)
        // e1.prime.lit = var;
        e1.prime = get_SFDD(v->lt, var);  // normalization
        e1.sub.constant = 1;
        e2.prime = get_SFDD2(v->lt, var);
        e2.sub.constant = -1;
    } else {
        // rule 1.(b)
        e1.prime.constant = 1;
        // e1.sub.lit = var;
        e1.sub = get_SFDD(v->rt, var);  // normalization
        e2.prime = get_SFDD2(v->rt, 0);
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
SFDD Manager::get_SFDD2(const Vtree* v, const int rht) const {
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
        e.prime = get_SFDD2(v->lt);
        e.sub = get_SFDD2(v->rt);
        sfdd.elements.push_back(e);
    } else if (rht == 0) {
        // rule 3
        Element e1, e2;
        e1.prime.constant = 1;
        e1.sub = get_SFDD2(v->rt, 0);
        e2.prime = e1.sub;
        e2.sub = get_SFDD2(v->rt);
        sfdd.elements.push_back(e1);
        sfdd.elements.push_back(e2);
    } else {
        // rule 2
        Element e1, e2;
        set<int> lt_vars = v->lt->get_variables();
        if (lt_vars.find(rht) != lt_vars.end()) {
            // rule 2.(a)
            // e1.prime.lit = rht;
            e1.prime = get_SFDD(v->lt, rht);  // normalization
            e1.sub = get_SFDD2(v->rt, 0);
            e2.prime = get_SFDD2(v->rt, rht);
            e2.sub = get_SFDD2(v->rt);
        } else {
            // rule 2.(b)
            e1.prime.constant = 1;
            e1.sub = get_SFDD2(v->rt, rht);
            e2.prime = get_SFDD2(v->lt, 0);
            e2.sub = get_SFDD2(v->rt);
        }
        sfdd.elements.push_back(e1);
        sfdd.elements.push_back(e2);
    }
    return sfdd;
}
