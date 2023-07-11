#include "codealg.h"

#include <assert.h>

int charToInt(char c){
    int ascii = (int)c;
    if(ascii == (int)' ')
        return 26;
    assert(ascii >= (int)'a' && ascii <= (int)'z');
    return ascii - (int)'a';
}