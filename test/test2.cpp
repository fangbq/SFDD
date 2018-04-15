/* 
 * File: test2.cpp
 * Author: Biqing Fang
 * This file is used to generates random vtrees
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
    if (argv[2]) var_no = atoi(argv[2]);
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    for (int i = 0; i < 100; ++i) {
        milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        srand ( unsigned ( ms.count() ) );
        random_shuffle(vars_order.begin(), vars_order.end(), myrandom);
        // Vtree* v = new Vtree(1, var_no*2-1, vars_order, RANDOM_TREE);
        string test_name = argv[1];
        // v->save_vtree_file("test/"+test_name+"/"+test_name+"_"+to_string(i)+".vtree");
    }

    return 0;
}
