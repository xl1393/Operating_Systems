#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <string>
#include <vector>
#include "Event.h"
using namespace std;

class EventBigger{
    bool reverse;
public:
    EventBigger(const bool& revparam=false);

    bool operator() (const Event& A, const Event& B);
};
#endif