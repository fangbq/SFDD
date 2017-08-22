/* 
 * File: test0.cpp
 * Author: Biqing Fang
 * this test file is used to test following basic cases:
 * 1. f=0
 * 2. f=1
 * 3. f=x1
 * 4. f=-x1
 * 5. f=x2
 */

#include <iostream>
#include <string>
// #include "reader.h"
#include "SFDD.h"

int main(int argc, char** argv) {

    vector<int> vars_order;
    int var_no = 18;
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree* v = new Vtree(1, var_no*2-1, vars_order);
    v->save_file_as_dot("vtree");

    Manager m(v);

    SFDD sfdd1 = m.sfddZero();
    sfdd1.save_file_as_dot("f=0");

    SFDD sfdd2 = m.sfddOne();
    sfdd2.save_file_as_dot("f=1");

    SFDD sfdd3 = m.sfddVar(18);
    sfdd3.save_file_as_dot("f=x18");

    SFDD sfdd4 = m.sfddVar(17);
    sfdd4.save_file_as_dot("f=x17");

    SFDD sfdd5 = sfdd3.Xor(sfdd4, m, true);
    sfdd5.save_file_as_dot("f=x17_xor_x18");
cout << "haha 1" << endl;

    SFDD sfdd6 = sfdd3.And(sfdd4, m, true);
    sfdd6.save_file_as_dot("f=x17_and_x18");
cout << "haha 2" << endl;

    SFDD sfdd7 = sfdd3.Or(sfdd4, m, true);
    sfdd7.save_file_as_dot("f=x17_or_x18");
cout << "haha 3" << endl;


    return 0;
}
