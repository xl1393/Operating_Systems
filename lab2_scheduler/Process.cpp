#include "Process.h"

Process::Process(int _AT, int _TC, int _CB, int _IO, int _PID, int _PRI, int _REMTIME) 
    :AT(_AT), TC(_TC), CB(_CB), IO(_IO), process_id(_PID), static_prio(_PID), rem_time(_REMTIME)
    {}

bool Prio_Compare(Process& A, Process& B) {
    return A.priority > B.priority;
}