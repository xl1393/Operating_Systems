#include "scheduler.h"

Scheduler::Scheduler() { IDLE = true; }

Scheduler::is_idle() { return IDLE; }

Process* FCFS_Sched::pick_process(vector<Process*>& ready_queue) {
    Process* picked = ready_queue.front();
    ready_queue.pop_front();
    return picked;
}

Process* LCFS_Sched::pick_process(vector<Process*>& ready_queue) {
    Process* picked = ready_queue.back();
    ready_queue.pop_back();
    return picked;
}

Process* SJF_Sched::pick_process(vector<Process*>& ready_queue) {
    stable_sort(ready_queue.begin(), ready_queue.end(), SJF_queue_comparator);
    Process* picked = ready_queue.front();
    ready_queue.pop_front();
    return picked;
}

Process* RR_Sched::pick_process(vector<Process*>& ready_queue) {
    stable_sort(ready_queue.begin(), ready_queue.end(), queue_prio_comparator);
    Process* picked = ready_queue.front();
    ready_queue.pop_front();
    return picked;
}
