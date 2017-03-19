#include "Event.h"

Event::Event(int _time_stamp, int _PID, int _exec_time, string _oldstate, string _newstate, int _gene_order)
    :time_stamp(_time_stamp), process_id(_PID), exec_time(_exec_time), oldstate(_oldstate), newstate(_newstate), gene_order(_gene_order) {}

int Event::Transition() {
    if (oldstate == "CREATED" && newstate == "READY") return 1;
    else if (oldstate == "READY" && newstate == "RUNNG") return 2;
    else if (oldstate == "RUNNG" && newstate == "BLOCK") return 3;
    else if (oldstate == "BLOCK" && newstate == "READY") return 4;
    else if (oldstate == "RUNNG" && newstate == "READY") return 5;
    else return 0;
}