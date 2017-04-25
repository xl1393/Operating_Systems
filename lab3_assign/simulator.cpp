#include "simulator.h"

simulator::simulator(int max_memory, const opts _opt, const char* filename, char algo) {
    vector<int> randnum;
    ifstream file;
    file.open(filename, ios::in);
    string sub;
    file>> sub;
    while (file >> sub) {
        randnum.push_back(stoi(sub));
    }
    opt = _opt;
    for (int i = 0; i < 64; i++) {
        pte tmp = {
            0,
            0,
            0,
            0,
            0,
        };
        pt.push_back(tmp);
    }
    switch(algo) {
        case 'N':
            // NRU
            replacer = new NRU(pt, randnum, max_memory);
            break;
        case 'r':
            // Random
            replacer = new Random(pt, randnum, max_memory);
            break;
        case 'f':
            // FIFO
            replacer = new FIFO(pt, randnum, max_memory);
            break;
        case 's':
            // SC
            replacer = new SC(pt, randnum, max_memory);
            break;
        case 'c':
            // Clock (Physical)
            replacer = new Clock(false, pt, randnum, max_memory);
            break;
        case 'X':
            // Clock (Virtual)
            replacer = new Clock(true, pt, randnum, max_memory);
            break;
        case 'a':
            // Aging (Physical)
            replacer = new Aging(false, pt, randnum, max_memory);
            break;
        case 'Y':
            // Aging (Virtual)
            replacer = new Aging(true, pt, randnum, max_memory);
    }
    instr_count = 0;
    stats = stat({0,0,0,0,0});
}

void simulator::parser_instr(string s) {
    istringstream iss(s);
    iss >> read >> page_i;
    if (page_i >= 64) { cout << "exceed max memory (page should be in [0,39])"; exit(1);}
}

void simulator::do_instr() {
    // print inst
    if (opt['O']) cout << "==> instr: " << read << " " << page_i << endl;
    if (pt[page_i].PRESENT == 0) {
        if(!replacer->isfull()) {
            if (opt['O']) printf("%d: ZERO\t\t%d\n", instr_count, (int)replacer->frameinfo.size());
            stats.zeros++;
            if (opt['O']) printf("%d: MAP\t%d\t%d\n", instr_count, page_i, (int)replacer->frameinfo.size());
            stats.maps++;
            if (read) replacer->Update(page_i, 1, 1);
            else replacer->Update(page_i, 1, 0);
            pt[page_i].frame = (int)replacer->frameinfo.size();
            pt[page_i].PRESENT = 1;
            replacer->frameinfo.push_back(page_i);
        }
        else {
            // page fault
            int change = replacer->ChangeFrame();
            if (opt['O']) 
                if (opt['a']) { replacer->print_a(); cout << '\n'; }
            int f_i = pt[change].frame;
            if (opt['O']) printf("%d: UNMAP\t%d\t%d\n", instr_count, change, f_i);
            stats.unmaps++;
            if (pt[change].MODIFIED) {
                if (opt['O']) printf("%d: OUT\t%d\t%d\n", instr_count, change, f_i);
                stats.outs++;
                pt[change].PAGEDOUT = 1;
            }
            if (pt[page_i].PAGEDOUT == 1) {
                if (opt['O']) printf("%d: IN\t%d\t%d\n", instr_count, page_i, f_i);
                stats.ins++;
            }
            else {
                if (opt['O']) printf("%d: ZERO\t\t%d\n", instr_count, f_i);
                stats.zeros++;
            }
            if (opt['O']) printf("%d: MAP\t%d\t%d\n", instr_count, page_i, f_i);
            stats.maps++;
            if (read) replacer->Update(page_i, 1, 1);
            else replacer->Update(page_i, 1, 0);
            pt[page_i].frame = f_i;
            pt[page_i].PRESENT = 1;
            pt[change].PRESENT = 0;
            replacer->frameinfo[f_i] = page_i;
        }
    }
    else {
        if (read) replacer->Update(page_i, 1, 1);
        else if (pt[page_i].MODIFIED) replacer->Update(page_i, 1, 1);
        else replacer->Update(page_i, 1, 0);
    }
    if (opt['O']) {
        if (opt['p']) replacer->print_P();
        if (opt['f']) {
            replacer->print_f();
            if (opt['p']) { replacer->print_p();}
            cout << '\n';
        }
    }
}