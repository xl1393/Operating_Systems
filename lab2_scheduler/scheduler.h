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
        virtual Process* pick_process(vector<Process*>& ready_queue) = 0;
};

class FCFS_Sched: public Schduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue);
}

class LCFS_Sched: public Schduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue);
}

class SJF_Sched: public Schduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue);
}

class RR_Sched: public Schduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue);
}

class PRIO_Sched: public Schduler {
    public:
        
        virtual Process* pick_process(vector<Process*>& ready_queue);
}
