#ifndef IO_OPERATOR_H
#define IO_OPERATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* abs */
#include "IO.h"
using namespace std;

class IO_algo {
private:
    int current_position;

public:
    IO_algo();
    virtual void add_IO(IO&) = 0; // add request to state
    virtual IO* next_IO() = 0; // Get next IO
    virtual void issue() = 0; // Update the state, normaly after calling next IO
    virtual bool finish() = 0; // Check if finish
    void Update_position(int current_pos);
    int Get_position();
};

class FIFO: public IO_algo {
private:
    vector<IO*> FIFO_queue;

public:
    FIFO();
    virtual void add_IO(IO&);
    virtual IO* next_IO();
    virtual void issue();
    virtual bool finish();
};

class SSTF: public IO_algo {
private:
    vector<IO*> SSTF_vector;
    int delete_id;

public:
    SSTF();
    virtual void add_IO(IO&);
    virtual IO* next_IO();
    virtual void issue();
    virtual bool finish();
};

class SCAN: public IO_algo {
private:
    vector<IO*> SCAN_vector;
    int direct;
    int delete_id;

public:
    SCAN();
    virtual void add_IO(IO&);
    virtual IO* next_IO();
    virtual void issue();
    virtual bool finish();
};

class CSCAN: public IO_algo {
private:
    vector<IO*> CSCAN_vector;
    int delete_id;

public:
    CSCAN();
    virtual void add_IO(IO&);
    virtual IO* next_IO();
    virtual void issue();
    virtual bool finish();
};

class FSCAN: public IO_algo {
private:
    vector<IO*> queue_1;
    vector<IO*> queue_2;
    int direct;
    int delete_id;

public:
    FSCAN();
    virtual void add_IO(IO&);
    virtual IO* next_IO();
    virtual void issue();
    virtual bool finish();
};

#endif