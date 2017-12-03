#ifndef SFDD_H
#define SFDD_H

#include "sfdd_common.h"
#include "sfdd_manager.h"

using namespace std;


namespace sfdd {

class Vtree;
class Sfdd;
class SfddElement;

class Sfdd {
private:
    addr_t addr_;
    SfddManager& mngr_;
public:
    Sfdd(const addr_t addr, SfddManager& manager) : 
        addr_(addr), mngr_(manager) { 
        // mngr_.inc_zsddnode_refcount_at(addr_);
    } 
    Sfdd(const Sfdd& obj) : 
        addr_(obj.addr_), mngr_(obj.mngr_) {
        // mngr_.inc_zsddnode_refcount_at(addr_);
    }
    Sfdd& operator=(const Sfdd& obj) {
        if (addr_ == obj.addr_) return *this;
        // mngr_.dec_zsddnode_refcount_at(addr_);
        addr_ = obj.addr_;
        // mngr_.inc_zsddnode_refcount_at(addr_);
        return *this;
    }
    unsigned long long int size() const {
        return mngr_.size(addr_);
    }
    addr_t addr() const { return addr_; }
};


class SfddElement {
public:
    Sfdd prime;
    Sfdd sub;
public:
    SfddElement() {}
    SfddElement(const SfddElement& e) { prime = e.prime; sub = e.sub; }
    inline bool equals(const SfddElement & e) const {
        return prime==e.prime && sub==e.sub;
    };
    void print(int indent, int counter) const;
    void print_dot(fstream & out_dot, int depth, string e_name) const;
};

}  // namespace sfdd

#endif
