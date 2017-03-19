#include "Simulator.h"

Simulator::Simulator(const char* randomfilename) {
    ofs = 0;
    event_count = 0;
    sim_time = 0;
    ifstream file;
    file.open(randomfilename, ios::in);
    string sub;
    file>>sub; // first line indicate random number amount 
    total_rand = stoi(sub);
    while (file >> sub) {
        randval.push_back(stoi(sub));
    }
}

int Simulator::myrandom(int burst) {
    int randomnum;
    randomnum 1 + (randval[ofs++] % burst);
    if (ofs == total_rand) ofs = 0; 
    return randomnum;
}

void Simulator::Init_Process(const char* filename) {
    ifstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        cout << "Open file failed!" << endl;
        exit(1);
    }
    lines = 0;
    current_AT = 0;
    current_TC = 0;
    current_CB = 0;
    current_IO = 0;
    while(file >> current_AT >> current_TC >> current_CB >> current_IO) {
        Process new_process(current_AT, current_TC, current_CB, lines, myrandom(4), current_TC);
        process_list.push_back(new_process);
        put_event(current_AT, PID, 0, "CREATED", "READY",event_count);
        event_count++;
        lines++;
    }
}

void Simulator::put_event(int _curr_time, int _PID, int _exec_time, string _oldstate, string _newstate) {
    Event newEvent(_curr_time, _PID, _exec_time, _oldstate, _newstate, event_count);
    event_queue.push(newEvent);
    event_count++;
}

int Simulator::Init_Scheduler(const string& schedname, const bool& _verbose, const int& _quantum = 10000) {
    verbose = _verbose;
    if (schedname == "F") {
        sched = new FCFS_Sched();
        sched_type = "FCFS";
    }
    else if (schedname == "L") {
        sched = new LCFS_Sched();
        sched_type = "LCFS";
    }
    else if (schedname == "S") {
        sched = new SJF_Sched();
        sched_type = "SJF";
    }
    else if (schedname == "RR") {
        sched = new RR_Sched();
        quantum = _quantum;
        sched_type = "RR";
    }
    else if (schedname == "PRIO") {
        sched = new PRIO_Sched();
        quantum = _quantum;
        sched_type = "PRIO";
    }
    else return 0;
    return 1;
}

void Simulator::handle_event(Event& event) {
    Process* p = &(process_list[event.process_id]);
    if (verbose) {
        cout << event.time_stamp << " " << event.process_id << " " << sim_time - p->most_recent_time << ": ";
    }
    switch (event.Transition()) {
        case CREATED_TO_READY:
            if (verbose) {
                cout << event.oldstate << " -> " << event.newstate << endl;
            }
            // add process to ready queue
            p->most_recent_time = event.time_stamp;
            ready_queue.push_back(p);
            break;
        case READY_TO_RUNNG:
            if (verbose) {
                cout << event.oldstate << " -> " << event.newstate << ": cb=" << p->current_cb << " rem=" << p->rem_time << " prio=" << p->dynamic_prio << endl;
            }
            p->most_recent_time = event.time_stamp;
            if (quantum < p->current_cb) {
                p->current_cb -= quantum;
                p->rem_time -= quantum;
                put_event(event.time_stamp + quantum, p->process_id, quantum, 'RUNNG', 'READY');
            }
            else {
                if (p->rem_time == p->current_cb) {
                    // No need to consider smaller condition cause current_cb must be no less than rem_time
                    put_event(event.time_stamp + p->rem_time, p->process_id, p->rem_time, 'DONE', 'DONE');
                    p->FT = event.time_stamp + p->rem_time;
                    p->TT = p->FT - p->AT;
                    p->IT = p->TT - p->TC - p->CW;
                }
                else {
                    put_event(event.time_stamp + p->current_cb, p->process_id, p->current_cb, 'RUNNG', 'BLOCK');
                    p->rem_time -= p->current_CB;
                    p->current_cb = 0;
                }
            }
            break;
        case RUNNG_TO_BLOCK:
            int new_io = myrandom(p->IO);
            p->current_ib = new_io;
            if (verbose) {
                cout << event.oldstate << " -> " << event.newstate << ": ib=" << p->current_ib << " rem=" << p->rem_time << endl;
            }
            put_event(event.time_stamp + new_io, p->process_id, new_io, "BLOCK", "READY");
            sched->IDLE = true;
            break;
        case BLOCK_TO_READY:
            if (verbose) {
                cout << event.oldstate << " -> " <<event.newstate << endl;
            }
            p->current_ib = 0;
            p->dynamic_prio = static_prio - 1;
            p->most_recent_time = event.time_stamp;
            ready_queue.push_back(p);
            break;
        case RUNNG_TO_READY:
            if (verbose) {
                cout << event.oldstate << " -> " << event.newstate << ": cb=" << p->current_cb << " rem=" << p->rem_time << " prio=" << p->dynamic_prio << endl;
            }
            p->dynamic_prio--;
            p->most_recent_time = event.time_stamp;
            ready_queue.push_back(p);
            sched->IDLE = true;
            break;
        case DONE:
            if (verbose) {
                cjout << "Done" << endl;
            }
            sched->IDLE = true;
            break;
    }
    sim_time = event.time_stamp;
}

void Simulator::run_action() {
    if (sched->is_idle() && !ready_queue.empty()) {
        Process* picked_p = sched->pick_process(ready_queue);
        // Check if cpu burst remain
        if (picked_p->current_cb == 0) {
            int new_cb = myrandom(picked_p->CB);
            if (new_cb >= picked_p->rem_time) new_cb = picked_p->rem_time;
            picked_p->current_cb = new_cb;
        }
        put_event(sim_time, picked_p->process_id, sim_time - picked_p->most_recent_time, "READY", "RUNNG");
        picked_p->CW += sim_time - picked_p->most_recent_time;
        sched->IDLE = false;
    }
}

void Simulator::run_all() {
    while (!event_queue.empty()) {
        Event curr_event = event_queue.top();
        event_queue.pop();
        handle_event(curr_event);
        if (!event_queue.empty() && event_queue.top().time_stamp > sim_time) {
            run_action();
        }
        if (event_queue.empty() && !ready_queue.empty()) {
            run_action();
        }
    }
}

void Simulator::print_summary() {
    cout << sched_type;
    if (quantum != 10000) {
        cout << " " << quantum;
    }
    cout << endl;
    //Information of every process
    for (int i = 0; i < process_list.size(); i++) {
        Process p = process_list[i];
        cout << expand_4_digit(i) << ": " << p.AT << " " << p.TC << " " << p.CB << " " << p.IO << " " << p.static_prio << " | " << p.FT << " " << p.TT << " " << p.IT << " " << p.CW << endl;
    }
    // Sumary
    cout << "SUM: "
}