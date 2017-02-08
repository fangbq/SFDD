#include <iostream>
#include <string>
#include "reader.h"
#include "SFDD.h"

using namespace std;

int main(int argc, char** argv) {
    Reader rd(argv[1]);
    Vtree v(1, 17);
    v.print();
    return 0;
}