#pragma once

#include <iostream>

using std::cout;
using std::endl;
///
/// Defines directives for logging with differenet levels,
/// and controllable indentation of debug and trace logs, which
///   makes the the call hierarchy level visible (by prefixing the log messages with some spaces) 

///
/// log_deb(msg1 << msg2 << ...<< msg_k); will write TAB,msg1,msg2...,msg_k  to the standard output (if LOG_SEVERITY<=2),
///   where TAB consists of space characters. 
/// TAB width(number of spaces) can be increased/decreased  by calling LOG_indent(n):
///
/// eg. call LOG_indent(3): aat the start of a method, 
/// (and use some calls to LOG_deb or LOG_trace in the function)
///  and LOG_indent(-3) at its the end.
///


/// LOG_SEVERITY: log level: minimum of severites for which logs should be printed.
///   error,warn,info,deb,trace are the severity levels in decreasing order. 
///   (their severities are  5,4,3,2,1 in order)
///   logs that have a severity level below LOG_SEVERITY won't be printed.
#define LOG_SEVERITY 2
#define LOG_error(asd) (LOG_SEVERITY>5 ? cout << "" : cout << asd << endl);
#define LOG_warn(asd) (LOG_SEVERITY>4 ? cout << "" : cout << asd << endl);
#define LOG_info(asd) (LOG_SEVERITY>3 ? cout << "" : cout << asd << endl);
#define LOG_deb(asd) (LOG_SEVERITY>2 ? cout << "" : cout << asd << endl);
#define LOG_trace(asd) (LOG_SEVERITY>1 ? cout << "" : cout << logging::TRACE_TAB << asd << endl);
/// increase indent level by num(can be negative in which case the new level will be less)
/// indentation only occures in  trace logs
#define LOG_indent(num) (LOG_SEVERITY>2 ? 0 : (int)(logging::TRACE_TAB += num));


/// Dont use anything in the logging namespace. use the macro definitions instead
namespace logging{
// helper declarations(private interface)
// struct for logging  indentation control
struct TABT{
    int s=0;
    TABT& operator+=(int inc);
    TABT& operator-=(int decr);
    constexpr explicit operator int const(){
        return 0;
    }
};
std::ostream& operator<<(std::ostream& os, const TABT& t);
// debug indentation level declaration
extern TABT TRACE_TAB;
} // namespace logging