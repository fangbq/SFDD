/* 
 * File: test2.cpp
 * Author: Biqing Fang
 * this test file is used to test ⊕ operator:
 * 1. f=0⊕0
 * 2. f=0⊕1     6. f=1⊕1
 * 3. f=0⊕x1    7. f=1⊕x1   10. f=x1⊕x1
 * 4. f=0⊕-x1   8. f=1⊕-x1  11. f=x1⊕-x1  13. f=-x1⊕-x1
 * 5. f=0⊕x2    9. f=1⊕x2   12. f=x1⊕x2   14. f=-x1⊕x2   15. f=x2⊕x2
 */

#include <iostream>
#include <string>
#include "reader.h"
#include "SFDD.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {

    vector<int> vars_order;
    int var_no = 18;
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    for (int i = 0; i < 100; ++i) {
        milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        srand ( unsigned ( ms.count() ) );
        random_shuffle(vars_order.begin(), vars_order.end(), myrandom);
        Vtree* v = new Vtree(1, var_no*2-1, vars_order, RANDOM_TREE);
        v->save_vtree_file("test/s27/s27_"+to_string(i)+".vtree");
    }

    return 0;
}
