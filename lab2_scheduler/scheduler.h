#include <iostream>
#include <vector>
#include <algorithm>
#include "utility.h"
#include "Process.h"

using namespace std;

class Scheduler {
    public:
        bool IDLE; // idle status of scheduler
        // Constructor
        Scheduler();
        bool is_idle();
        virtual Process* pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue) = 0;
};

class FCFS_Sched: public Scheduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue);
};

class LCFS_Sched: public Scheduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue);
};

class SJF_Sched: public Scheduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue);
};

class RR_Sched: public Scheduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue);
};

class PRIO_Sched: public Scheduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue);
};
