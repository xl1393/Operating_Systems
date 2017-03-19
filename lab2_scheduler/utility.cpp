#include "Utility.h"

EventBigger::EventBigger(const bool& revparam=false):reverse(revparam) {}
SJF_queue_comparator::SJF_queue_comparator(const bool& revparam=false):reverse(revparam) {}
RR_queue_prio_comparator::RR_queue_prio_comparator(const bool& revparam=false):reverse(revparam) {}

bool EventBigger::operator() (const Event& A, const Event& B) {
    if (A.time_stamp !=  B.time_stamp) {
        return A.time_stamp > B.time_stamp;
    }
    return A.gene_order > B.gene_order;
}

bool SJF_queue_comparator::operator() (Process* A, Process* B) {
    return A->rem_time < B->rem_time;
}

bool queue_prio_comparator::operator() (Process* A, Process* B) {
    return A->dynamic_prio > B->dynamic_prio;
}

string expand_4_digit(int num) {
    string tmp = to_string(10000 + num);
    tmp.erase(0,1);
    return tmp;
}