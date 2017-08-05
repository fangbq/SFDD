/* 
 * File: test1.cpp
 * Author: Biqing Fang
 * this test file is used to test following basic cases:
 * 1. f=¬0
 * 2. f=¬1
 * 3. f=¬x1
 */

#include <iostream>
#include <string>
#include "reader.h"
#include "SFDD.h"

using namespace std;

int main(int argc, char** argv) {

    vector<int> vars_order;
    int var_no = 4;
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree* v = new Vtree(1, var_no, vars_order);

    Manager m(v);

    fstream f;

    /* @test f=¬0 */
    SFDD sfdd1 = m.sfddZero();
    f.open("dotG/test1/f=¬0.dot", fstream::out | fstream::trunc);
    sfdd1.Not(m).print_dot(f, true);
    f.close();
    // sfdd1.print();

    /* @test f=¬1 */
    SFDD sfdd2 = m.sfddOne();
    f.open("dotG/test1/f=¬1.dot", fstream::out | fstream::trunc);
    sfdd2.Not(m).print_dot(f, true);
    f.close();
    
    /* @test f=¬x1 */
    SFDD sfdd3 = m.sfddVar(1);
    f.open("dotG/test1/f=¬x1.dot", fstream::out | fstream::trunc);
    sfdd3.Not(m).print_dot(f, true);
    f.close();

    return 0;
}
