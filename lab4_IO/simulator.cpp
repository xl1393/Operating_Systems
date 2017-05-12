#include "simulator.h"

simulator::simulator(char algo, const vector<string> s): current_time(0), current_track(0), current_OP(0) {
    switch(algo) {
        case 'i':
            ALGO = new FIFO();
            break;
        case 'j':
            ALGO = new SSTF();
            break;
        case 's':
            ALGO = new SCAN();
            break;
        case 'c':
            ALGO = new CSCAN();
            break;
        case 'f':
            ALGO = new FSCAN();
            break;
    }
    for (int i = 0; i < s.size(); i++) {
        istringstream iss(s[i]);
        int time_step;
        int track;
        iss >> time_step >> track;
        IO new_IO;
        new_IO.time_step = time_step;
        new_IO.track = track;
        new_IO.OP = i;
        IO_OP.push_back(new_IO);
    }
}

void simulator::sim_all() {
    int id = 0;
    while (id < IO_OP.size()) {
        if(id == 0) {
            current_time = IO_OP[id].time_step;
            finish_time = current_time + IO_OP[id].track;
            current_track = IO_OP[id].track;
            IO_OP[id].movement = IO_OP[id].track;
            IO_OP[id].issue_time = current_time;
            IO_OP[id].finish_time = finish_time;
            IO_tmp = &(IO_OP[id]);
            // printf("%d:\t%d add %d\n", current_time, IO_OP[id].OP, IO_OP[id].track);
            // printf("%d:\t%d issue %d %d\n", current_time, IO_OP[id].OP, IO_OP[id].track, 0);
            id++;
        }
        else {
            if(IO_OP[id].time_step < finish_time) {
                // Just add IO to buffer
                current_time = IO_OP[id].time_step;
                // printf("%d:\t%d add %d\n", current_time, IO_OP[id].OP, IO_OP[id].track);
                ALGO->add_IO(IO_OP[id]);
                id++;
            }
            else if (IO_OP[id].time_step == finish_time) {
                // first add to IO queue
                current_time = IO_OP[id].time_step;
                // printf("%d:\t%d add %d\n", current_time, IO_OP[id].OP, IO_OP[id].track);
                ALGO->add_IO(IO_OP[id]);
                id++;
                // Then get next one
                ALGO->Update_position(current_track);
                IO* candidate = ALGO->next_IO();
                current_time = finish_time;
                // printf("%d:\t%d finish %d\n", current_time, IO_tmp->OP, IO_tmp->finish_time - IO_tmp->time_step);
                // printf("%d:\t%d issue %d %d\n", current_time, candidate->OP, candidate->track, current_track);
                candidate->issue_time = current_time;
                candidate->movement = abs(candidate->track - current_track);
                candidate->finish_time = current_time + candidate->movement;
                finish_time = candidate->finish_time;
                current_track = candidate->track;
                IO_tmp = candidate;
                ALGO->issue();
            }
            else {
                if (ALGO->finish()) {
                    // do as the first time
                    // printf("%d:\t%d finish %d\n", finish_time, IO_tmp->OP, IO_tmp->finish_time - IO_tmp->time_step);
                    current_time = IO_OP[id].time_step;
                    IO_OP[id].movement = abs(IO_OP[id].track - current_track);
                    IO_OP[id].finish_time = current_time + IO_OP[id].movement;
                    finish_time = IO_OP[id].finish_time;
                    // printf("%d:\t%d add %d\n", current_time, IO_OP[id].OP, IO_OP[id].track);
                    // printf("%d:\t%d issue %d %d\n", current_time, IO_OP[id].OP, IO_OP[id].track, current_track);
                    current_track = IO_OP[id].track;
                    IO_OP[id].issue_time = current_time;
                    IO_tmp = &(IO_OP[id]);
                    id++;
                }
                else {
                    ALGO->Update_position(current_track);
                    IO* candidate = ALGO->next_IO();
                    current_time = finish_time;
                    // printf("%d:\t%d finish %d\n", current_time, IO_tmp->OP, IO_tmp->finish_time - IO_tmp->time_step);
                    // printf("%d:\t%d issue %d %d\n", current_time, candidate->OP, candidate->track, current_track);
                    candidate->issue_time = current_time;
                    candidate->movement = abs(candidate->track - current_track);
                    candidate->finish_time = current_time + candidate->movement;
                    finish_time = candidate->finish_time;
                    current_track = candidate->track;
                    IO_tmp = candidate;
                    ALGO->issue();
                }
            }
        }
    }
    while(!ALGO->finish()) {
        ALGO->Update_position(current_track);
        IO* candidate = ALGO->next_IO();
        current_time = finish_time;
        // printf("%d:\t%d finish %d\n", current_time, IO_tmp->OP, IO_tmp->finish_time - IO_tmp->time_step);
        // printf("%d:\t%d issue %d %d\n", current_time, candidate->OP, candidate->track, current_track);
        candidate->issue_time = current_time;
        candidate->movement = abs(candidate->track - current_track);
        candidate->finish_time = current_time + candidate->movement;
        finish_time = candidate->finish_time;
        current_track = candidate->track;
        IO_tmp = candidate;
        ALGO->issue();
    }
}

void simulator::print_sum() {
    int tot_movement = 0;
    double avg_turnaround = 0;
    double avg_waittime = 0;
    int max_waittime = IO_OP[0].issue_time - IO_OP[0].time_step;
    for (int i = 0; i < IO_OP.size(); i++) {
        // printf("%d\n", IO_OP[i].finish_time);
        tot_movement += IO_OP[i].movement;
        avg_turnaround += (double) (IO_OP[i].finish_time - IO_OP[i].time_step);
        avg_waittime += (double) (IO_OP[i].issue_time - IO_OP[i].time_step);
        if (max_waittime < IO_OP[i].issue_time - IO_OP[i].time_step) {
            max_waittime = IO_OP[i].issue_time - IO_OP[i].time_step;
        }
    }
    avg_turnaround /= IO_OP.size();
    avg_waittime /= IO_OP.size();
    printf("SUM: %d %d %.2lf %.2lf %d\n",
        finish_time,
        tot_movement,
        avg_turnaround,
        avg_waittime,
        max_waittime);
}
