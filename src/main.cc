/*
 * File: main.cpp
 * Author: Biqing Fang
 * this test file is used to test a case from benchmarks:
 * 1. f=0+0
 * 2. f=0+1     6. f=1+1
 * 3. f=0+x1    7. f=1+x1   10. f=x1+x1
 * 4. f=0+-x1   8. f=1+-x1  11. f=x1+-x1  13. f=-x1+-x1
 * 5. f=0+x4    9. f=1+x4   12. f=x1+x4   14. f=-x1+x4   15. f=x4+x4
 */
#include <sys/time.h>
#include <sys/resource.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <assert.h>
#include <chrono>
#include "sfdd.h"
#include <time.h>
using namespace std;
using namespace sfdd;


std::map<int, int> get_index_by_var = {{0,0}};  // x1 stored as "1", x4 stored as "4"...
                                           // indexs of true and false both are "0"


int main(int argc, char** argv) {


    Manager manager;
    // cout << "Benchmarks\tInputs\tOutputs\tWires\tNode Count\tRuntime (s)" << endl;  // table header
    clock_t start = clock();
    // addr_t fml = manager.cnf_to_sfdd(argv[1]);  // for CNF file
    unordered_set<addr_t> fmls = manager.verilog_to_sfdds(argv[1], argv[2]);  // for Verilog file
    clock_t finish = clock();
    double ptime = (double)(finish - start) / CLOCKS_PER_SEC;  // runtime
    cout.setf(ios::showpoint);
    cout.precision(4);
    cout.setf(ios::fixed);
    cout << manager.size(fmls) << "\t\t" << ptime << endl;


/*
    vector<int> vars_order;
    int var_no = 4;
    if (argv[1])
        var_no = stoi(argv[1]);
    for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);

    Vtree v(1, var_no*2-1, vars_order);
    // v.save_dot_file("vtree");
    // v.print();

    Manager m(v);

    addr_t x1 = m.sfddVar(1);
    addr_t x2 = m.sfddVar(2);
    addr_t x3 = m.sfddVar(3);
    addr_t x4 = m.sfddVar(4);

    addr_t x5 = m.Xor(m.Xor(m.And(x1, x2), m.And(x1, x4)), m.And(x3, x4));
    m.print(x5);

    // m.print_unique_table();
    // m.print_cache_table();
    cout << m.size(x5) << endl;
*/

    return 0;
}
