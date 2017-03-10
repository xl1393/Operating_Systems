#include <iostream>
#include "Process.h"

using namespace std;

class Scheduler {
    public:
        // Constructor
        Scheduler();
        virtual Process pick_event(Process* )
};

class FCFS_Sched: public Schduler {
    public:
        
        FCFS_Sched();
        

        

}