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
    int var_no = 18;
    if (argv[1])
        var_no = stoi(argv[1]);
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree v(1, var_no*2-1, vars_order);
    v.save_file_as_dot("vtree");

    Manager m(v);

    SFDD x1 = m.sfddVar(17);
    
    SFDD x2 = m.sfddVar(7);

    SFDD x3 = m.sfddVar(16);

    SFDD x4 = x1.Or(x2, m, true);
    x4.save_file_as_dot("f=x17+x7");

    // SFDD x5 = x4.Or(x3, m, true);
    // x5.save_file_as_dot("f=x17+x7+x16");
    return 0;
}
// 