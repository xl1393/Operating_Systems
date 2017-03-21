#include "utility.h"

EventBigger::EventBigger(const bool& revparam):reverse(revparam) {}

bool EventBigger::operator() (const Event& A, const Event& B) {
    if (A.time_stamp !=  B.time_stamp) {
        return A.time_stamp > B.time_stamp;
    }
    return A.gene_order > B.gene_order;
}

bool SJF_queue_comparator(Process* A, Process* B) {
    return A->rem_time < B->rem_time;
}

bool queue_prio_comparator(Process* A, Process* B) {
    return A->dynamic_prio > B->dynamic_prio;
}

int add_CPU_time(const int& init, const Process& p) {
    return init + p.TC;
}

int add_IO_time(const int& init, const Process& p) {
    return init + p.IT;
}

int add_Turnaround_time(const int& init, const Process& p) {
    return init + p.TT;
}

int add_CPU_wait(const int& init, const Process& p) {
    return init + p.CW;
}

string expand_4_digit(int num) {
    string tmp = to_string(10000 + num);
    tmp.erase(0,1);
    return tmp;
}