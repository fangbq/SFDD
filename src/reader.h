#ifndef READER_H
#define READER_H

#include "define.h"
#include "SFDD.h"


class Reader {
public:
    string problem_name = "None";
    int var_num = 0;
    int cla_num = 0;
    vector<vector<int> > fml;
public:
    Reader(const char* pro_file);
};

#endif
