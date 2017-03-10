#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Process {
    public:
        //property of Process
        int AT;
        int TC;
        int CB;
        int IO;
        int process_id;
        int priority;
        int rem_time;
        int FT; // Finish Time
        int TT; // Turnaround Time
        int IT; // I/O Time
        int CW; // CPU Waiting Time

        // Constructor
        Process(int _AT, int _TC, int _CB, int _IO, int _PID, int _PRI, int _REMTIME);
        
        friend bool Prio_Compare(Process& A, Process& B);
};

#endif