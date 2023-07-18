#pragma once
#include <iostream>

namespace logging{
// TABT for tabulation while printing 
// makes the the recursion level visible (by using tabs) while logging recursive calls
struct TABT{
    int s=0;
    TABT& operator+=(int inc);
    TABT& operator-=(int decr);
};
// TABT instance to use change logging tab size
extern TABT TAB;

std::ostream& operator<<(std::ostream& os, const TABT& t);
}
