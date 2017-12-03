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
#include "reader.h"
#include "SFDD.h"
#include <time.h>
// #include <stdlib.h>

using namespace std;

int main(int argc, char** argv) {
    // read *.cnf
    ifstream infile(argv[1], ios::in);
    string file_name = static_cast<string>(argv[1]);
    cout << file_name.substr(file_name.find_last_of("/")+1, file_name.length()-file_name.find_last_of("/")) << "\t";
    if(!infile)
    {
        cerr << "open infile error!" << endl;
        exit(1);
    }
    string line;
    int var_no = 0, col_no = 0;  // Number of variables and number of clauses
    while (!infile.eof()) {
        getline(infile, line);

        if (line.length() == 0 || line[0] == 'c')
            ; //cout << "IGNORE LINE\n";
        else {
            var_no = stoi(line.substr(6, line.find_last_of(' ')-6));
            col_no = stoi(line.substr(line.find_last_of(' ')+1, line.length()-line.find_last_of(' ')));
            break;
        }
    }
    cout << var_no << "  " << col_no << "    ";

    // vector<int> vars_order;  // order
    // for (int i = 1; i <= var_no; ++i) vars_order.push_back(i);
    // Vtree v(1, var_no*2-1, vars_order);  // vtree
    Vtree v(argv[2]);
    v.save_file_as_dot("vtree");
    v.print();
    Manager m(v);  // manager

    SFDD fml;
    // struct rusage r_usage;
    int  clause_counter = 1;
    clock_t start = clock();
    vector<SFDD> sfdds;  // not because out memory
    for(int line = 0; line < col_no; ++line)  //read every line number, and save as a clause
    {
        SFDD clause;
        while (true) {
            int var;
            infile >> var;
            if (var == 0) break;
            SFDD tmp_var = m.sfddVar(var);
            // cout << "var: " << var << " done" << endl;
            clause = clause.Or(tmp_var, m);
            clause.print();
            // getrusage(RUSAGE_SELF, &r_usage);
            // cout << "Memory usage = " << r_usage.ru_maxrss << endl;
            // clause.save_file_as_dot(to_string(clause_counter)+"after"+to_string(var));
            // cout << endl;
        }
        // clause.save_file_as_dot("clause_"+to_string(clause_counter++));
        fml = fml.And(clause, m, true, clause_counter);
        // fml.print();
        // fml.save_file_as_dot("fml_"+to_string(clause_counter-1));
        cout << "clause : " << clause_counter++ << " done; size : " << fml.size() << endl;
    }
    clock_t finish = clock();
    double ptime = (double)(finish - start) / CLOCKS_PER_SEC;  //BDD time
    cout.setf(ios::showpoint);
    cout.precision(4);
    cout.setf(ios::fixed);
    cout << ptime << "  " << fml.size() << endl;
    fml.save_file_as_dot("s1");
    // fstream f;
    // f.open("dotG/iscas89/f=s1.dot", fstream::out | fstream::trunc);
    // fml.print_dot(f, true);
    // f.close();

    return 0;
}
