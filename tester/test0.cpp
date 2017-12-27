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
    int var_no = 2;
    if (argv[1])
        var_no = stoi(argv[1]);
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree v(1, var_no*2-1, vars_order);
    v.save_dot_file("vtree");
    v.print();

    Manager m(v);

    SFDD x1 = m.sfddVar(1);
    x1.print();
    
    SFDD x2 = m.sfddVar(2);
    x2.print();

    // SFDD x3 = m.sfddVar(16);

    SFDD x4 = x1.Xor(x2, m);
    x4.print();
    return 0;
}
// 