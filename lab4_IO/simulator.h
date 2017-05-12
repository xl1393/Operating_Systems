#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>     /* abs */
#include "IO.h"
#include "IO_Operator.h"
using namespace std;

class simulator {
private:
    IO_algo* ALGO;
    IO* IO_tmp;
    vector<IO> IO_OP;
    int current_time; // the end of the current process time
    int finish_time;
    int current_track;
    int current_OP;

public:
    simulator(char algo, const vector<string> s);
//     void parser_line(string s);
//     bool take_action(bool just_do_it);
    void sim_all();
    void print_sum();
};

#endif
