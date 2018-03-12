/* 
 * File: test0.cpp
 * Author: Biqing Fang
 * this test file is used to test following basic cases:
 * 1. f=0
 * 2. f=1
 * 3. f=x10
 * 4. f=-x10
 * 5. f=x10
 */

#include <iostream>
#include <string>
#include "SFDD.h"

int main(int argc, char** argv) {

    vector<int> vars_order;
    int var_no = 4;
    if (argv[1])
        var_no = stoi(argv[1]);
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree v(1, var_no*2-1, vars_order);
    // v.save_dot_file("vtree");
    v.print();

    Manager m(v);

    SFDD x1 = m.sfddVar(1);
    
    SFDD x2 = m.sfddVar(3);

    SFDD x3 = x1.apply(OR, x2, m);
    // cout << x3.size(m) << endl;

    // m.print_sfdd_nodes();

    // sort(x3.elements.begin(), x3.elements.end(), less_than_element());
    x3.print(m);
    // SFDD x4 = x1.And(x3, m);
    // x4.print(m);

    // m.print_sfdd_nodes();

    // if (m.sfdd_nodes_[2]==m.sfdd_nodes_[6]) cout << "haha" << endl;
    // m.print_unique_table();
    // m.print_cache_table();
    cout << x3.size(m) << endl;
    return 0;
}
// 