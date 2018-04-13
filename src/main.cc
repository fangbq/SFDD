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
    // read *.cnf
    std::ifstream infile(argv[1], ios::in);
    std::string file_name = static_cast<std::string>(argv[1]);
    cout << file_name.substr(file_name.find_last_of("/")+1, file_name.length()-file_name.find_last_of("/")) << "\t";
    if(!infile)
    {
        cerr << argc << "open infile error!" << endl;
        exit(1);
    }
    std::string line;
    int var_no = 0, col_no = 0;  // Number of variables and number of clauses
    while (!infile.eof()) {
        std::getline(infile, line);

        if (line.length() == 0 || line[0] == 'c')
            ; //cout << "IGNORE LINE\n";
        else {
            var_no = stoi(line.substr(6, line.find_last_of(' ')-6));  //  p cnf a b; this line can't end with ' '
            col_no = stoi(line.substr(line.find_last_of(' ')+1, line.length()-line.find_last_of(' ')));
            break;
        }
    }
    cout << var_no << "  " << col_no << "    ";

    Vtree v;

    if (argv[2])
        v = *new Vtree(argv[2]);
    else {
        std::vector<int> vars_order;
        for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);
        v = *new Vtree(1, var_no*2-1, vars_order);
    }
    // v.save_vtree_file("s27_ balanced.vtree");

    Manager m(v);  // manager

    addr_t fml = m.true_;
    // struct rusage r_usage;
    int  clause_counter = 1;
    clock_t start = clock();
    for(int line = 0; line < col_no; ++line)  //read every line number, and save as a clause
    {
        addr_t clause = m.false_;
        while (true) {
            int var;
            infile >> var;
            if (var == 0) break;
            addr_t tmp_var = m.sfddVar(var);
            clause = m.Or(clause, tmp_var);
            // m.print_sfdd_nodes();
            // m.print_unique_table();
            // clause.save_file_as_dot(to_string(clause_counter)+"after"+to_string(var));
            // cout << endl;
        }
        // clause.save_file_as_dot("clause_"+to_string(clause_counter++));
        fml = m.And(fml, clause);
        // fml.print();
        // fml.save_file_as_dot("fml_"+to_string(clause_counter-1));
        cout << "clause : " << clause_counter++ << " done; " << m.size(fml) << endl;
    }
    // m.print(fml);
    clock_t finish = clock();
    double ptime = (double)(finish - start) / CLOCKS_PER_SEC;  //BDD time
    cout.setf(ios::showpoint);
    cout.precision(4);
    cout.setf(ios::fixed);
    cout << ptime << "  " << m.size(fml) << endl;
    // fml.save_file_as_dot("sddpaper_eg");
    // fstream f;
    // f.open("dotG/iscas89/f=s1.dot", fstream::out | fstream::trunc);
    // fml.print_dot(f, true);
    // f.close();

    return 0;
}
