#ifndef REPLACE_H
#define REPLACE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "pte.h"
using namespace std;

class replaces {
    
public:
    vector<pte*> pt;
    vector<int> frameinfo;
    vector<int> rands;
    int max_memory;
    
    replaces(vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame() = 0;
    virtual void Update(int page_index, int r_a, int m_a) = 0;
    virtual void print_p() = 0;
    virtual void print_a() = 0;
    void print_P();
    void print_f();
    bool isfull();
};

class NRU: public replaces {
public:
    vector<vector<int> > array; // Store four different class
    int rofs;
    int replace_count;
    vector<int>::iterator index;
    int lowest_class;
    int selix;
    vector<int> print_array;

    NRU(vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame();
    virtual void Update(int page_index, int r_a, int m_a);
    void addpage(int row, int page_index);
    void copyarray(vector<int>& source);
    virtual void print_p();
    virtual void print_a();
};

class Random: public replaces {
public:
    int rofs;
    int replace_count;
    int changed_frame;
    vector<int>::iterator index;
    
    Random(vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame();
    virtual void Update(int page_index, int r_a, int m_a);
    virtual void print_p();
    virtual void print_a();
};

class FIFO: public replaces {
public:
    vector<int> array;

    FIFO(vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame();
    virtual void Update(int page_index, int r_a, int m_a);
    virtual void print_a();
    virtual void print_p();
};

class SC: public replaces {
public:
    vector<int> array;

    SC(vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame();
    virtual void Update(int page_index, int r_a, int m_a);
    virtual void print_a();
    virtual void print_p();
};

class Clock: public replaces {
public:
    vector<int> array;
    int clock_i;
    bool _virtual;

    Clock(bool base_virtual, vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame();
    virtual void Update(int page_index, int r_a, int m_a);
    virtual void print_a();
    virtual void print_p();
};

class Aging: public replaces {
public:
    vector<unsigned long int> counter;
    bool _virtual;
    int min_pte;
    int current_page;
    
    Aging(bool base_virtual, vector<pte>& _pt, const vector<int>& _rands, int _max);
    virtual int ChangeFrame();
    virtual void Update(int page_index, int r_a, int m_a);
    void updateaging();
    void reset();
    int findmini();
    virtual void print_a();
    virtual void print_p();
};

#endif
