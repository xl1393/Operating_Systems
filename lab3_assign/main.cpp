#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "simulator.h"
#include "Replace.h"
#include "pte.h"
using namespace std;

int main(int argc, char **argv) {
    int opt_ch;
    char algo;
    opts optt ( {{'O', false},
                 {'P', false},
                 {'F', false},
                 {'S', false},
                 {'p', false},
                 {'a', false},
                 {'f', false}
                } );
    opterr = 0;
    int max_frame;
    string line;
    string tmp;
    while ((opt_ch = getopt(argc, argv, "a:o:f:")) != -1) {
        if (opt_ch == 'a') algo = optarg[0];
        else if (opt_ch == 'o') {
            tmp = optarg;
            for (int i = 0; i < tmp.size(); i++) { 
                if (tmp[i] == '[' || tmp[i] == ']') continue;
                else optt[tmp[i]] = true; 
            }
        }
        else if (opt_ch == 'f') {
            tmp = optarg;
            max_frame = stoi(tmp);
        }
    }

    simulator sim(max_frame, optt, argv[optind+1], algo);
    ifstream infile(argv[optind]);
    while (getline(infile, line)) {
        if (line[0] != '#') {
            sim.parser_instr(line);
            sim.do_instr();
            sim.instr_count++;
        }
    }
    // print summary
    if (optt['P']) sim.replacer->print_P();
    if (optt['F']) { sim.replacer->print_f(); cout << '\n'; }
    if (optt['S']) {
        long long totalcost;
        totalcost = (long long)sim.instr_count + ((long long)sim.stats.maps + (long long)sim.stats.unmaps) * 400 + 3000 * ((long long)sim.stats.ins + (long long)sim.stats.outs) + 150 * (long long)sim.stats.zeros;
        printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
            sim.instr_count, sim.stats.unmaps, sim.stats.maps, sim.stats.ins, sim.stats.outs, sim.stats.zeros, totalcost);
    }
    return 0;
}