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

    // Vtree v(argv[1]);
    // v.print();
    // fstream f;
    // f.open("dotG/test_vtree_reader/vtree.dot", fstream::out | fstream::trunc);
    // v.print_dot(f, true);
    // f.close();

    // Manager m(v);

    // SFDD x1 = m.sfddVar(1);
    // x1.print();
    
    // SFDD x2 = m.sfddVar(2);
    // x2.print();

    // SFDD x3 = x1.Xor(x2, m);
    // x3.print();
    // SFDD x4 = x1.And(x2, m, true);
    // x4.print();
    // SFDD x5 = x1.Or(x2, m, true);
    // x5.print();
    // x3.save_file_as_dot("f=x1+x2");

    return 0;
}
