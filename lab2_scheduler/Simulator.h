#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "Process.h"
#include "Event.h"
#include "utility.h"
using namespace std;


class Simulator {
    public:
        // Constructor
        int current_AT;
        int current_TC;
        int current_CB;
        int current_IO;
        int line_counter;
        priority_queue<Event, vector<Event>, EventBigger> event_queue; 
        vector<Process> process_list;
        vector<int> randval;
        vector<Process*> ready_queue;
        int total_rand;
        int ofs;
        Simulator(const char* randomfilename);
        int myrandom(int burst);
        void Init_Process(const char* filename);
        
        void put_event(int _curr_AT, int _PID, int )
};

#endif