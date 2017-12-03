#include "sfdd_node.h"

extern map<int, int> get_index_by_var;

namespace sfdd {

Sfdd& SfddNode::operator=(const SfddNode& sfdd_node) {
    literal_ = sfdd.literal_;
    decomp_ = sfdd.decomp_;
    vtree_index = sfdd.vtree_index;
    return *this;
}
/*
void SfddNode::print(int indent) const {
    if (decomp_.empty()) {
        for (int i = 0; i < indent; ++i) cout << " ";
        if (literal_ < 2) {
            // cout << literal_ << endl;
            cout << literal_ << " ~ " << vtree_index << endl;
        } else {
            if (is_negative()) cout << "-";
            // cout << "x" << literal_/2 << endl;
            cout << "x" << literal_/2 << " ~ " << vtree_index << endl;
        }
        return;
    }
    for (int i = 0; i < indent; ++i) cout << " ";
    cout << "Dec " << vtree_index << ":" << endl;
    int counter = 1;
    for (vector<SfddElement>::const_iterator e = decomp_.begin();
    e != decomp_.end(); ++e) {
        e->print(indent+1, counter++);
    }
    return;
}

void SfddNode::print_dot(fstream & out_dot, bool root, int depth, string dec_name) const {
    if (root) {
        out_dot << "digraph G {" << endl;
        if (is_terminal()) {
            if (is_constant()) {
                out_dot << "\t" << literal_ << " [shape=record, label=\"" \
                << literal_ << "\"]" << endl << "}";
                // << literal_ << " ~ " << vtree_index << "\"]" << endl << "}";
            } else {
                out_dot << "\t";
                if (is_negative()) out_dot << "neg";
                out_dot << "x" << literal_/2 << " [shape=record, label=\"";
                if (is_negative()) out_dot << "-";
                out_dot << "x" << literal_/2 << "\"]" << endl << "}";
                // out_dot << "x" << literal_/2 << " ~ " << vtree_index << "\"]" << endl << "}";
            }
            return;
        }
    }
    if (decomp_.empty()) {
        if (literal_ < 2) {
            out_dot << literal_;
            // out_dot << literal_ << "~" << vtree_index;
        } else {
            if (is_negative()) out_dot << "-";
            out_dot << "x" << literal_/2;
            // out_dot << "x" << literal_/2 << "~" << vtree_index;
        }
        return;
    }
    ++depth;
    out_dot << "\t" << dec_name << " [shape=circle, label=\"" << vtree_index << "\"]" << endl;
    string e_name = "Ele_" + to_string(depth) + "_1";
    for (vector<SfddElement>::const_iterator e = decomp_.begin();
    e != decomp_.end(); ++e) {
        e_name = check_dec_name(e_name);
        out_dot << "\t" << dec_name << " -> " << e_name << endl;
        e->print_dot(out_dot, depth, e_name);
    }
    if (root) out_dot << "}" << endl;
}

void SfddNode::save_file_as_dot(const string f_name) const {
    fstream f;
    f.open("dotG/test0/"+f_name+".dot", fstream::out | fstream::trunc);
    print_dot(f, true);
    f.close();
}

void Element::print(const int indent, int counter) const {
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
}*/

} // namespace sfdd