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
    int var_no = 4;
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree* v = new Vtree(1, var_no*2-1, vars_order);
    v->print();
    v->save_file_as_dot("vtree");

    Manager m(v);

    /* @test f=0 */
    SFDD sfdd1 = m.sfddZero();
    sfdd1.save_file_as_dot("f=0");

    /* @test f=1 */
    SFDD sfdd2 = m.sfddOne();
    sfdd2.save_file_as_dot("f=1");
    
    /* @test f=x1 */
    SFDD sfdd3 = m.sfddVar(4);
    sfdd3.save_file_as_dot("f=x4");

    /* @test f=-x1 */
    SFDD sfdd4 = m.sfddVar(-1);
    sfdd4.save_file_as_dot("f=-x1.dot");

    /* @test f=x4^-x1 */
    SFDD sfdd7 = sfdd4.Xor(sfdd3, m);
    sfdd7.save_file_as_dot("f=x4^-x1");

    /* @test f=x2 */
    SFDD sfdd5 = m.sfddVar(2);
    sfdd5.save_file_as_dot("f=x2");
    cout << sfdd5.vtree_index << endl;
cout << "ha 1" << endl;
    sfdd5.normalized(4, m).save_file_as_dot("f=nml(x2)");
cout << "ha 2" << endl;
    sfdd7.normalized(4, m).save_file_as_dot("f=nml(x4^-x1)");
cout << "ha 3" << endl;
    sfdd5.Xor(sfdd7, m).save_file_as_dot("f=x2^(x4^-x1)");
cout << "ha 4" << endl;

    return 0;
}
