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
    Vtree* v = new Vtree(1, 4, vars_order);
    v->print();
    cout << endl;

    // @test vtree-get_variables
    set<int> vars = v->get_variables();
    cout << "all variables" << endl;
    for (set<int>::iterator v = vars.begin(); v != vars.end(); ++v) {
        cout << *v << " ";
    }
    cout << endl << endl;

    // @test the approach of compiling (note8)
    Manager mg;
    SFDD sfdd1 = mg.get_SFDD(v, 1);
    sfdd1.print();
    cout << endl;
    SFDD sfdd2 = mg.get_SFDD(v, 2);
    sfdd2.print();
    cout << endl;

    return 0;
}
