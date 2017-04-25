#include "Replace.h"

replaces::replaces(vector<pte>& _pt, const vector<int>& _rands, int _max) {
    max_memory = _max;
    rands = _rands;
    for (int i = 0; i < _pt.size(); i++) {
        pt.push_back(&_pt[i]);
    }
}

bool replaces::isfull() {
    return  frameinfo.size() == max_memory;
}


void replaces::print_P() {
    // Print page info
    for (int i = 0; i < pt.size(); i++) {
        if (pt[i]->PRESENT == 0) {
            if (pt[i]->PAGEDOUT) {cout << "#";}
            else { cout << "*"; }
        }
        else {
            cout << i << ":";
            if (pt[i]->REFERENCE) {cout << "R";}
            else { cout << "-";}
            if (pt[i]->MODIFIED) {cout << "M";}
            else { cout << "-";}
            if (pt[i]->PAGEDOUT) {cout << "S";}
            else { cout << "-";}
        }
        cout << " ";
    }
    cout << '\n';
}

void replaces::print_f() {
    int i;
    for (i = 0; i < frameinfo.size(); i++) {
        cout << frameinfo[i] << " ";
    }
    while (i < max_memory) {
        cout << "*" << " ";
        i++;
    }
}

// NRU algo
NRU::NRU(vector<pte>& _pt, const vector<int>& _rands, int _max): replaces(_pt, _rands, _max) {
    rofs = 0;
    for(int i = 0; i < 4; i++) {
        vector<int> tmp;
        array.push_back(tmp);
    }
    replace_count = 0;
}


int NRU::ChangeFrame() {
    // count plus 1 before implementing replacement operator
    
    int randnum;
    int page;
    for(int i = 0; i < 4; i++) {
        if(array[i].size() != 0) {
            copyarray(array[i]);
            randnum = rands[rofs++] % array[i].size();
            if (rofs == rands.size()) rofs = 0;
            page = array[i][randnum];
            lowest_class = i;
            selix = randnum;
            array[i].erase(array[i].begin() + randnum);
            break;
        }
    }
    replace_count++;
    if (replace_count == 10) {
        // transfer class 2 to class 0
        if (array[2].size() != 0) {
            int i = 0;
            while(i < array[2].size()) {
                if (pt[array[2][i]]->PRESENT) { 
                    pt[array[2][i]]->REFERENCE = 0; 
                    addpage(0, array[2][i]);
                    array[2].erase(array[2].begin() + i);
                }
                else i++;
            }
        }
        // transfer class 3 to class 1
        if (array[3].size() != 0) {
            int i = 0;
            while(i < array[3].size()) {
                if (pt[array[3][i]]->PRESENT) { 
                    pt[array[3][i]]->REFERENCE = 0; 
                    addpage(1, array[3][i]);
                    array[3].erase(array[3].begin() + i);
                }
                else i++;
            }
        }
        replace_count = 0;
    }
    return page;
}

void NRU::copyarray(vector<int>& source) {
    vector<int> tmp;
    for (int i = 0; i < source.size(); i++) {
        tmp.push_back(source[i]);
    }
    print_array = tmp;
}

void NRU::Update(int page_index, int r_a, int m_a) {
    int class_index;
    int class_a;
    int r,m;
    r = pt[page_index]->REFERENCE;
    m = pt[page_index]->MODIFIED;
    if (r==0 && m==0) {class_index = 0;}
    if (r==0 && m==1) {class_index = 1;}
    if (r==1 && m==0) {class_index = 2;}
    if (r==1 && m==1) {class_index = 3;}
    if (r_a==0 && m_a==0) {class_a = 0;}
    if (r_a==0 && m_a==1) {class_a = 1;}
    if (r_a==1 && m_a==0) {class_a = 2;}
    if (r_a==1 && m_a==1) {class_a = 3;}
    index = find(array[class_index].begin(), array[class_index].end(), page_index);
    if(index != array[class_index].end()) array[class_index].erase(index);
    addpage(class_a, page_index);
    pt[page_index]->REFERENCE = r_a;
    pt[page_index]->MODIFIED = m_a;
}

void NRU::addpage(int row, int page_index) {
    //keep the array become a sorted version
    int i;
    for (i = 0; i < array[row].size(); i++) {
        if (array[row][i] > page_index) {
            break;
        }
    }
    array[row].insert(array[row].begin() + i, page_index);
}

void NRU::print_p() {}

void NRU::print_a() {
    if (replace_count == 0) cout << " @@ reset NRU refbits while walking PTE\n";
    cout << " @@ lowest_class=" << lowest_class << ": selidx=" << selix << " from";
    for (int i = 0; i < print_array.size(); i++ ) {
        cout << " " << print_array[i];
    }
}

// Random algo
Random::Random(vector<pte>& _pt, const vector<int>& _rands, int _max): replaces(_pt, _rands, _max) {
    rofs = 0;
}

int Random::ChangeFrame() {
    int i;
    i = rands[rofs++] % frameinfo.size();
    if (rofs == rands.size()) rofs = 0;
    changed_frame = i;
    return frameinfo[i];
}

void Random::Update(int page_index, int r_a, int m_a) {
    pt[page_index]->REFERENCE = r_a;
    pt[page_index]->MODIFIED = m_a;
}

void Random::print_a() {}

void Random::print_p() {}

// FIFO algo
FIFO::FIFO(vector<pte>& _pt, const vector<int>& _rands, int _max): replaces(_pt, _rands, _max) {}

int FIFO::ChangeFrame() {
    int page_index;
    page_index = array.front();
    array.erase(array.begin());
    return page_index;
}

void FIFO::Update(int page_index, int r_a, int m_a) {
    if (find(array.begin(), array.end(), page_index) == array.end()) array.push_back(page_index);
    pt[page_index]->REFERENCE = r_a;
    pt[page_index]->MODIFIED = m_a;
}

void FIFO::print_a() {}

void FIFO::print_p() {
    cout << " ||";
    for (int i = 0; i < array.size(); i++) {
        cout << " " << pt[array[i]]->frame;
    }
}

// SC algo
SC::SC(vector<pte>& _pt, const vector<int>& _rands, int _max): replaces(_pt, _rands, _max) {}

int SC::ChangeFrame() {
    int frame_index;
    frame_index = array.front();
    while(pt[frameinfo[frame_index]]->REFERENCE != 0) {
        array.erase(array.begin());
        array.push_back(frame_index);
        pt[frameinfo[frame_index]]->REFERENCE = 0;
        frame_index = array.front();
    }
    array.erase(array.begin());
    array.push_back(frame_index);
    return frameinfo[frame_index];
}

void SC::Update(int page_index, int r_a, int m_a) {
    if (!isfull())
        if (pt[page_index]->PRESENT == 0) array.push_back((int)frameinfo.size());
    // if (find(array.begin(), array.end(), page_index) == array.end()) array.push_back(page_index);
    pt[page_index]->REFERENCE = r_a;
    pt[page_index]->MODIFIED = m_a;
}

void SC::print_a() {}

void SC::print_p() {
    cout << " ||";
    for (int i = 0; i < array.size(); i++) {
        cout << " " << array[i];
    }
}

// Clock algo
Clock::Clock(bool base_virtual, vector<pte>& _pt, const vector<int>& _rands, int _max): replaces(_pt, _rands, _max) {
    clock_i = 0;
    _virtual = base_virtual;
}

int Clock::ChangeFrame() {
    if (_virtual) {
        int page_index;
        if (clock_i == pt.size()) clock_i = 0;
        while (pt[clock_i]->REFERENCE != 0 || pt[clock_i]->PRESENT == 0) {
            if (pt[clock_i]->PRESENT == 1) pt[clock_i]->REFERENCE = 0;
            clock_i++;
            if (clock_i == pt.size()) clock_i = 0;
        }
        page_index = clock_i;
        return page_index;
    }
    else{
        int page_index;
        if (clock_i == array.size()) clock_i = 0;
        while (pt[array[clock_i]]->REFERENCE != 0) {
            pt[array[clock_i]]->REFERENCE = 0;
            clock_i++;
            if (clock_i == array.size()) {
                clock_i = 0;
            }
        }
        page_index = array[clock_i];
        array.erase(array.begin()+clock_i);
        return page_index;
    }
}

void Clock::Update(int page_index, int r_a, int m_a) {
    if (_virtual) {
        pt[page_index]->REFERENCE = r_a;
        pt[page_index]->MODIFIED = m_a;
    }
    else {
        if (find(array.begin(), array.end(), page_index) == array.end()) {
            // page fault or create
            array.insert(array.begin() + clock_i, page_index);
            clock_i++;
        }
        // if (isfull()) {
        //     array.push_back(page_index);
        // }
        // else {
        //     array[clock_i] = page_index;
        //     clock_i++;
        //     if (clock_i == array.size()) {
        //         clock_i = 0;
        //     }
        // }
        pt[page_index]->REFERENCE = r_a;
        pt[page_index]->MODIFIED = m_a;
    }
}

void Clock::print_p() {
    cout << " || hand = " << clock_i + 1;
}

void Clock::print_a() {}

// Aging algo
Aging::Aging(bool base_virtual, vector<pte>& _pt, const vector<int>& _rands, int _max): replaces(_pt, _rands, _max) {
    for (int i = 0; i < pt.size(); i++) {
        counter.push_back(0);
    }
    _virtual = base_virtual;
}

int Aging::ChangeFrame() {
    int index;
    // Update
    updateaging();
    // reset
    reset();
    // find minimal
    index = findmini();
    if (_virtual) {
        unsigned long int result = counter[index];
        for (int i = 0; i < frameinfo.size(); i++) {
            if (counter[frameinfo[i]] == result) {
                if (index > frameinfo[i]) { index = frameinfo[i]; }
            }
        }
    }
    min_pte = index;
    return index;
}

void Aging::updateaging() {
    for (int i = 0; i < frameinfo.size(); i++) {
        counter[frameinfo[i]] = counter[frameinfo[i]]>>1;
        if (pt[frameinfo[i]]->REFERENCE == 1) { counter[frameinfo[i]] += 0x80000000; }
    }
}

void Aging::reset() {
    for (int i = 0; i < frameinfo.size(); i++) {
        pt[frameinfo[i]]->REFERENCE = 0;
    }
}

int Aging::findmini() {
    int index = 0;
    for (int i = 0; i < frameinfo.size(); i++ ) {
        // if (pt[frameinfo[i]]->PRESENT == 1)
        if (counter[frameinfo[i]] < counter[frameinfo[index]]) index = i;
    }
    return frameinfo[index];
}

void Aging::Update(int page_index, int r_a, int m_a) {
    current_page = page_index;
    pt[page_index]->REFERENCE = r_a;
    pt[page_index]->MODIFIED = m_a;
    if (pt[page_index]->PRESENT == 0) counter[page_index] = 0;
}

void Aging::print_a() {
    if (_virtual) {
        cout << " @@ ";
        for (int i = 0; i < counter.size(); i++) {
            if (pt[i]->PRESENT == 0) cout << " " << "*";
            else printf(" %d:%x", i, (int)counter[i]);
        }
        cout << '\n';
        printf(" @@ min_pte = %d age=%x", min_pte, (int)counter[min_pte]);
        // cout << " @@ min_pte= " << min_pte << " age=" << counter[min_pte];
    }
}

void Aging::print_p() {
    if (_virtual) {
        cout << " ||";
        for (int i = 0; i < counter.size(); i++) {
            if (pt[i]->PRESENT == 0) {
                cout << " " << "*";
            }
            else {
                // cout << " " << i << ":" << counter[i];
                printf(" %d:%x", i, (int)counter[i]);
            }
        }
    }
    else {
        cout << " ||";
        for (int i = 0; i < frameinfo.size(); i++) {
            // cout << " " << i << ":" << counter[frameinfo[i]];
            printf(" %d:%x", i, (int)counter[frameinfo[i]]);
        }
    }
}
