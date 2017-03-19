#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Event{
public:
    int time_stamp; // The time when the event finish the tansition (After this time, process will in new state)
    int process_id;
    int exec_time; // how long since the process in old state
    int gene_order; // order for event generation
    string oldstate;
    string newstate;
    
    // Constructor
    Event(int _time_stamp, int _PID, int _exec_time, string _oldstate, string _newstate, int _gene_order);

    // Method
    int Transition();
    // friend bool operator() (Event& A, Event& B);
};
#endif