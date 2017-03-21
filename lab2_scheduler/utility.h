#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <string>
#include <vector>
#include "Event.h"
#include "Process.h"
using namespace std;

class EventBigger{
    bool reverse;
public:
    EventBigger(const bool& revparam=false);

    bool operator() (const Event& A, const Event& B);
};

bool SJF_queue_comparator(Process* A, Process* B);

bool queue_prio_comparator(Process* A, Process* B);

int add_CPU_time(const int& init, const Process& p);

int add_IO_time(const int& init, const Process& p);

int add_Turnaround_time(const int& init, const Process& p);

int add_CPU_wait(const int& init, const Process& p);

string expand_4_digit(int num);
#endif