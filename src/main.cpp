#include <iostream>
#include <string>
#include "reader.h"
#include "SFDD.h"

using namespace std;

int main(int argc, char** argv) {
    Reader rd(argv[1]);

    // @test create vars order
    vector<int> vars_order;
    int var_no = 4;
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    // @test vtree-constuctor
    Vtree* v = new Vtree(1, var_no, vars_order);
    v->print();
    cout << endl;

    // @test vtree-get_variables
    // set<int> vars = v->get_variables();
    // cout << "all variables" << endl;
    // for (set<int>::iterator v = vars.begin(); v != vars.end(); ++v) {
    //     cout << *v << " ";
    // }
    // cout << endl << endl;

    // @test the approach of compiling (note8)
    Manager m(v);
    SFDD sfdd1 = m.sfddVar(v, 1);
    cout << "f=x1 :" << endl;
    sfdd1.print();

    fstream f;
    // @test print_dot(...)
    f.open("../dotG/f=x1.dot", fstream::out | fstream::trunc);
    sfdd1.print_dot(f, true);

    cout << endl;


    SFDD sfdd2 = m.sfddVar(v, 2);
    cout << "f=x2 :" << endl;
    sfdd2.print();
    cout << endl;
    // SFDD sfdd3 = m.sfddOne();
    // cout << "f=1 :" << endl;
    // sfdd3.expanded(m).print();
    // cout << endl;
    // SFDD sfdd4 = m.sfddZero();
    // cout << "f=0 :" << endl;
    // sfdd4.print();
    // cout << endl;

    // @test Intersection
    // cout << "f=x1 \\cap f=x2 :" << endl;
    // SFDD inter_sfdd = sfdd1.Intersection(sfdd2);
    // inter_sfdd.print();
    // cout << endl;

    // @test Xor
    cout << "f=x1 \\oplus f=x2 :" << endl;
    SFDD xor_sfdd = sfdd1.Xor(sfdd2, m);
    xor_sfdd.print();
    cout << endl;
    // cout << "f=x1 \\oplus f=1 :" << endl;
    // SFDD xor_sfdd2 = sfdd1.Xor(sfdd3, m);
    // xor_sfdd2.print();
    // cout << endl;
    // cout << "f=x1 \\and f=x2 :" << endl;
    // SFDD and_sfdd1 = sfdd1.And(sfdd2, m);
    // and_sfdd1.print();
    // cout << endl;

    f.close();

    return 0;
}
