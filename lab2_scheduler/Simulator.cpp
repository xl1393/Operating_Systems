#include "Simulator.h"

Simulator::Simulator(const char* randomfilename) {
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
    return 1 + (randval[ofs++] % burst);
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
        put_event(current_AT, PID, 0, "CREATED", "READY");
        lines++;
    }
}