#include "IO_Operator.h"

IO_algo::IO_algo(): current_position(0) {}

void IO_algo::Update_position(int current_pos) {
    current_position = current_pos;
}

int IO_algo::Get_position() {
    return current_position;
}

FIFO::FIFO() {}

void FIFO::add_IO(IO & entry_ptr) {
    IO* entry = &(entry_ptr);
    FIFO_queue.push_back(entry);
}

IO* FIFO::next_IO() {
    // for (int i = 0; i < FIFO_queue.size(); i++) {
    //     printf("%d ", FIFO_queue[i]->OP);
    // }
    // printf("\n");
    IO* tmp = FIFO_queue[0];
    return tmp;
}

void FIFO::issue() {
    FIFO_queue.erase(FIFO_queue.begin());
}

bool FIFO::finish() {
    return FIFO_queue.size() == 0;
}


SSTF::SSTF() {}

void SSTF::add_IO(IO& entry_ptr) {
    IO* entry = &(entry_ptr);
    SSTF_vector.push_back(entry);
}

IO* SSTF::next_IO() {
    int current_pos = IO_algo::Get_position();
    int current_dis = abs(SSTF_vector[0]->track - current_pos);
    delete_id = 0;
    int tmp;
    for (int i = 1; i < SSTF_vector.size(); i++) {
        tmp = abs(SSTF_vector[i]->track - current_pos);
        if (tmp < current_dis) {
            current_dis = tmp;
            delete_id = i;
        }
    }
    return SSTF_vector[delete_id];
}

void SSTF::issue() {
    SSTF_vector.erase(SSTF_vector.begin() + delete_id);
}

bool SSTF::finish() {
    return SSTF_vector.size() == 0;
}

SCAN::SCAN(): direct(1) {} //Initial direction is up

void SCAN::add_IO(IO& entry_ptr) { // sort insert
    IO* entry = &(entry_ptr);
    if (SCAN_vector.size() == 0) SCAN_vector.push_back(entry);
    else {
        for (int i = 0; i < SCAN_vector.size(); i++) {
            if(SCAN_vector[i]->track > entry->track) {
                SCAN_vector.insert(SCAN_vector.begin() + i, entry);
                return;
            }
        }
        SCAN_vector.push_back(entry);
    }
}

IO* SCAN::next_IO() {
    if (direct > 0) { // Means it is up
        int current_pos = IO_algo::Get_position();
        for (int i = 0; i < SCAN_vector.size(); i++) {
            if (SCAN_vector[i]->track >= current_pos) {
                delete_id = i;
                return SCAN_vector[i];
            }
        }
        direct = -1;
        delete_id = SCAN_vector.size() - 1;
        return SCAN_vector[delete_id];
    }
    if (direct < 0) {
        int current_pos = IO_algo::Get_position();
        for (int i = SCAN_vector.size() - 1; i >= 0; i--) {
            if (SCAN_vector[i]->track <= current_pos) {
                delete_id = i;
                return SCAN_vector[i];
            }
        }
        direct = 1;
        delete_id = 0;
        return SCAN_vector[0];
    }

}

void SCAN::issue() {
    SCAN_vector.erase(SCAN_vector.begin() + delete_id);
}

bool SCAN::finish() {
    return SCAN_vector.size() == 0;
}

CSCAN::CSCAN() {}

void CSCAN::add_IO(IO& entry_ptr) {
    IO* entry = &(entry_ptr);
    if (CSCAN_vector.size() == 0) CSCAN_vector.push_back(entry);
    else {
        for (int i = 0; i < CSCAN_vector.size(); i++) {
            if(CSCAN_vector[i]->track > entry->track) {
                CSCAN_vector.insert(CSCAN_vector.begin() + i, entry);
                return;
            }
        }
        CSCAN_vector.push_back(entry);
    }
}

IO* CSCAN::next_IO() {
    int current_pos = IO_algo::Get_position();
    for (int i = 0; i < CSCAN_vector.size(); i++) {
        if (CSCAN_vector[i]->track >= current_pos) {
            delete_id = i;
            return CSCAN_vector[i];
        }
    }
    delete_id = 0;
    return CSCAN_vector[0];
}

void CSCAN::issue() {
    CSCAN_vector.erase(CSCAN_vector.begin() + delete_id);
}

bool CSCAN::finish() {
    return CSCAN_vector.size() == 0;
}

FSCAN::FSCAN(): direct(1) {}

void FSCAN::add_IO(IO& entry_ptr) {
    IO* entry = &(entry_ptr);
    if (queue_2.size() == 0) queue_2.push_back(entry);
    else {
        for (int i = 0; i < queue_2.size(); i++) {
            if(queue_2[i]->track > entry->track) {
                queue_2.insert(queue_2.begin() + i, entry);
                return;
            }
        }
        queue_2.push_back(entry);
    }
}

IO* FSCAN::next_IO() {
    if (queue_1.size() == 0) {
        queue_1 = queue_2;
        queue_2.clear();
        direct = 1;
    }
    if (direct > 0) { // Means it is up
        int current_pos = IO_algo::Get_position();
        for (int i = 0; i < queue_1.size(); i++) {
            if (queue_1[i]->track >= current_pos) {
                delete_id = i;
                return queue_1[i];
            }
        }
        direct = -1;
        delete_id = queue_1.size() - 1;
        return queue_1[delete_id];
    }
    if (direct < 0) {
        int current_pos = IO_algo::Get_position();
        for (int i = queue_1.size() - 1; i >= 0; i--) {
            if (queue_1[i]->track <= current_pos) {
                delete_id = i;
                return queue_1[i];
            }
        }
        direct = 1;
        delete_id = 0;
        return queue_1[0];
    }
}

void FSCAN::issue() {
    queue_1.erase(queue_1.begin() + delete_id);
}

bool FSCAN::finish() {
    return (queue_1.size() == 0) && (queue_2.size() == 0);
}
