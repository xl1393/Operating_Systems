#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include "Event.h"
#include "Process.h"
#include "Simulator.h"

using namespace std;

int main(int argc, char **argv) {
    bool verbose = false;
    string parse_string;
    int opt_ch;
    int quantum = 10000;
    while ((opt_ch = getopt(argc, argv, "s:v")) != -1) {
        if (opt_ch == 'v') {
            verbose = true;
        }
        else if (opt_ch == 's') {
            if (optarg[0] == 'F') {
                parse_string = optarg;
            }
            else if (optarg[0] == 'L') {
                parse_string = optarg;
            }
            else if (optarg[0] == 'S') {
                parse_string = optarg;
            }
            else if (optarg[0] == 'P') {
                parse_string = optarg;
            }
            else if (optarg[0] == 'R') {
                parse_string = optarg;
            }
        }
    }

    Simulator sim(argv[optind+1]);
    sim.Init_Process(argv[optind]);
    sim.Init_Scheduler(parse_string, verbose);
    sim.run_all();
    sim.gather_info();
    sim.print_summary();

    return 0;
}