#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "pte.h"
#include "Replace.h"
using namespace std;

typedef unordered_map<char, bool> opts;

struct stat {
    int unmaps;
    int maps;
    int ins;
    int outs;
    int zeros;
};

class simulator {
public:
    vector<pte> pt;
    int instr_count;
    int read;
    int page_i;
    replaces* replacer;
    stat stats;
    opts opt;

    simulator(int max_memory, const opts _opt, const char* filename, char algo);
    void parser_instr(string s);
    void do_instr();
};

#endif
