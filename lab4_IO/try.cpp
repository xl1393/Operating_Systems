#include <iostream>
#include <vector>
using namespace std;

struct IO {
    int time_step=0;
    int track=0;
    int OP=0;
    int issue_time=0;
    int finish_time=0;
    int movement=0;
};

int main () {
    vector<IO*> a;
    IO tmp;
    tmp.time_step = 1;
    tmp.track = 2;
    tmp.OP = 3;
    a.push_back(&(tmp));
    printf("%d\n",a[0]->track);
    printf("%d\n",a[0]->track);
    printf("%d\n",a[0]->track);
    return 0;
}