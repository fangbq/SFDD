/* 
 * File: test3.cpp
 * Author: Biqing Fang
 * this test file is used to test · operator:
 * 1. f=0·0
 * 2. f=0·1     6. f=1·1
 * 3. f=0·x1    7. f=1·x1   10. f=x1·x1
 * 4. f=0·-x1   8. f=1·-x1  11. f=x1·-x1  13. f=-x1·-x1
 * 5. f=0·x2    9. f=1·x2   12. f=x1·x2   14. f=-x1·x2   15. f=x2·x2
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
    SFDD sfdd1 = m.sfddZero();
    SFDD sfdd2 = m.sfddOne();
    SFDD sfdd3 = m.sfddVar(1);
    SFDD sfdd4 = m.sfddVar(-1);
    SFDD sfdd5 = m.sfddVar(2);

    fstream f;

    /* @test 1-5 */
    f.open("dotG/test3/f=0·0.dot", fstream::out | fstream::trunc);
    sfdd1.And(sfdd1, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=0·1.dot", fstream::out | fstream::trunc);
    sfdd1.And(sfdd2, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=0·x1.dot", fstream::out | fstream::trunc);
    sfdd1.And(sfdd3, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=0·-x1.dot", fstream::out | fstream::trunc);
    sfdd1.And(sfdd4, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=0·x2.dot", fstream::out | fstream::trunc);
    sfdd1.And(sfdd5, m).print_dot(f, true);
    f.close();

    /* @test 6-9 */
    f.open("dotG/test3/f=1·1.dot", fstream::out | fstream::trunc);
    sfdd2.And(sfdd2, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=1·x1.dot", fstream::out | fstream::trunc);
    sfdd2.And(sfdd3, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=1·-x1.dot", fstream::out | fstream::trunc);
    sfdd2.And(sfdd4, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=1·x2.dot", fstream::out | fstream::trunc);
    sfdd2.And(sfdd5, m).print_dot(f, true);
    f.close();

    /* @test 10-12 */
    f.open("dotG/test3/f=x1·x1.dot", fstream::out | fstream::trunc);
    sfdd3.And(sfdd3, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=x1·-x1.dot", fstream::out | fstream::trunc);
    sfdd3.And(sfdd4, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=x1·x2.dot", fstream::out | fstream::trunc);
    sfdd3.And(sfdd5, m).print_dot(f, true);
    f.close();

    /* @test 13-14 */
    f.open("dotG/test3/f=-x1·-x1.dot", fstream::out | fstream::trunc);
    sfdd4.And(sfdd4, m).print_dot(f, true);
    f.close();
    f.open("dotG/test3/f=-x1·x2.dot", fstream::out | fstream::trunc);
    sfdd4.And(sfdd5, m).print_dot(f, true);
    f.close();

    /* @test 15 */
    f.open("dotG/test3/f=x2·x2.dot", fstream::out | fstream::trunc);
    sfdd5.And(sfdd5, m).print_dot(f, true);
    f.close();


    f.close();

    return 0;
}
