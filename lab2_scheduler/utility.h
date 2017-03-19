#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <string>
#include <vector>
#include "Event.h"
#incldue "Process.h"
using namespace std;

class EventBigger{
    bool reverse;
public:
    EventBigger(const bool& revparam=false);

    bool operator() (const Event& A, const Event& B);
};

class SJF_queue_comparator{
    bool reverse;
public:
    SJF_queue_comparator(const bool& revparam=false);

    bool operator() (Process* A, Process* B);
};

class queue_prio_comparator{
    bool reverse;
public:
    queue_prio_comparator(const bool& revparam=false);

    bool operator() (Process* A, Process* B);
};

string expand_4_digit(int num);
#endif