#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Event{
public:
    int time_stamp;
    int process_id;
    int exec_time;
    string oldstate;
    string newstate;
    
    // Constructor
    Event(int _time_stamp, int _PID, int _exec_time, string _oldstate, string _newstate);

    // Method
    int Transition();
    friend bool operator() (Event& A, Event& B);
};
#endif