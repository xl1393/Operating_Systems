#ifndef PTE_H
#define PTE_H
#include <iostream>
using namespace std;

struct pte {
    unsigned PRESENT:1;
    unsigned MODIFIED:1;
    unsigned REFERENCE:1;
    unsigned PAGEDOUT:1;
    unsigned frame:32;
};
#endif