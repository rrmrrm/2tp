#include "logging.h"

#include <iostream>

namespace logging{
TABT& TABT::operator+=(int inc){
    s += inc;
    if(s < 0)
        s = 0;
    return *this;
}
TABT& TABT::operator-=(int decr){
    s -= decr;
    if(s < 0)
        s = 0;
    return *this;
}
std::ostream& operator<<(std::ostream& os, const TABT& t){
    for(int i = 0 ; i < t.s ; ++i){
        os << ' ';
    }
    return os;
}
TABT TRACE_TAB;
} // namespace logging