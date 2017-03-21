#include "Process.h"

Process::Process(int _AT, int _TC, int _CB, int _IO, int _PID, int _PRI, int _REMTIME) 
    :AT(_AT), TC(_TC), CB(_CB), IO(_IO), process_id(_PID), static_prio(_PRI), rem_time(_REMTIME)
    {most_recent_time = _AT; dynamic_prio = _PRI - 1;}

bool Prio_Compare(Process& A, Process& B) {
    return A.dynamic_prio > B.dynamic_prio;
}