#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "IO_Operator.h"
#include "IO.h"
#include "simulator.h"
using namespace std;

int main(int argc, char ** argv) {
    int opt_ch;
    char algo;
    string line;
    while ((opt_ch = getopt(argc, argv, "s:")) != -1) {
        if (opt_ch == 's') algo = optarg[0];
    }
    ifstream infile(argv[optind]);
    vector<string> lines;
    while(getline(infile, line)) {
        if(line[0] != '#') {
            lines.push_back(line);
        }
    }
    simulator sim(algo, lines);
    sim.sim_all();
    sim.print_sum();
    return 0;
}