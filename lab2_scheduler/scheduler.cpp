#include "scheduler.h"

Scheduler::Scheduler() { IDLE = true; }

bool Scheduler::is_idle() { return IDLE; }

Process* FCFS_Sched::pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue) {
    Process* picked = ready_queue.front();
    ready_queue.erase(ready_queue.begin(), ready_queue.begin() + 1);
    return picked;
}

Process* LCFS_Sched::pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue) {
    Process* picked = ready_queue.back();
    ready_queue.erase(ready_queue.end()-1, ready_queue.end());
    return picked;
}

Process* SJF_Sched::pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue) {
    stable_sort(ready_queue.begin(), ready_queue.end(), SJF_queue_comparator);
    Process* picked = ready_queue.front();
    ready_queue.erase(ready_queue.begin(), ready_queue.begin() + 1);
    return picked;
}

Process* PRIO_Sched::pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue) {
    if (ready_queue.size() == 0) {
        vector<Process*> tmp;
        tmp = ready_queue;
        ready_queue = expired_queue;
        expired_queue = tmp;
    }
    int flags = 1;
    for (int i = 0; i < ready_queue.size(); i++) {
        if (ready_queue[i]->dynamic_prio >= 0) {flags = 0; break;}
    }
    if (flags == 1) {
        for (int j = 0; j < ready_queue.size(); j++) {
            ready_queue[j]->dynamic_prio = ready_queue[j]->static_prio - 1;
        }
    }
    stable_sort(ready_queue.begin(), ready_queue.end(), queue_prio_comparator);
    Process* picked = ready_queue.front();
    ready_queue.erase(ready_queue.begin(), ready_queue.begin() + 1);
    return picked;
}

Process* RR_Sched::pick_process(vector<Process*>& ready_queue, vector<Process*>& expired_queue) {
    //stable_sort(ready_queue.begin(), ready_queue.end(), queue_prio_comparator);
    Process* picked = ready_queue.front();
    ready_queue.erase(ready_queue.begin(), ready_queue.begin() + 1);
    return picked;
}