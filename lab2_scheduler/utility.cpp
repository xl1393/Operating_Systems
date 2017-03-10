#include "Utility.h"

bool EventBigger::operator() (const Event& A, const Event& B) {
    if (A.time_stamp !=  B.time_stamp) {
        return A.time_stamp > B.time_stamp;
    }
    return A.process_id > B.process_id;
}