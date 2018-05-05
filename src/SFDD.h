#ifndef SFDD_H
#define SFDD_H

#include "manager.h"
#include <iostream>
#include <string>
#include <set>


namespace sfdd {

class SFDD {
public:
    addr_t addr_ = -1;
    Manager& m;
public:
    SFDD(const addr_t addr, Manager& manager) : addr_(addr), m(manager) {}
    SFDD(const SFDD& sfdd) : addr_(sfdd.addr_), m(sfdd.m) {}
    SFDD& operator=(const SFDD& sfdd) {
        addr_ = sfdd.addr_;
        return *this;
    }
    addr_t addr() const { return addr_; }
    unsigned long long int size() const {
        return m.size(addr_);
    }
    void print() const {
        m.print(addr_);
    }
    // void print_dot(fstream & out_dot, const Manager& m, bool root = false, int depth = 0, string dec_name = "Dec_0_1") const;
    // void save_file_as_dot(const string f_name, const Manager& m) const;
};

} // namespace sfdd

#endif
