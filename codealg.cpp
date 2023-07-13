#include "codealg.h"

#include <assert.h>

int charToInt(char c){
    int ascii = (int)c;
    if(ascii == (int)' ')
        return 26;
    assert(ascii >= (int)'a' && ascii <= (int)'z');
    return ascii - (int)'a';
}
char intToChar(int val){
    if(val == 26)
        return ' ';
    assert(val >= 0 && val <= 25);
    char c = (int)'a' + val;
    return c;
}
string encode(const string& aMessage, const string& aKey){
    string code = aMessage;
    for(int i = 0 ; i < aMessage.size() ; ++i){
        int valCode = charToInt(aKey.at(i));
        int valMsg = charToInt(aMessage.at(i));
        int codeVal = (valCode + valMsg) % 27;
        // get the character from the character's code:
        code.at(i) = intToChar(codeVal);
    }
    return code;
}
string decode(const string& aCode, const string& aKey){
    string decoded = aCode;
    for(int i = 0 ; i < aCode.size() ; ++i){
        int valCode = charToInt(aKey.at(i));
        int valKey = charToInt(aCode.at(i));
        int codeVal = (valCode - valKey) % 27;
        // get the character from the character's code:
        decoded.at(i) = intToChar(codeVal);
    }
    return decoded;
}

string calcKey(const string& aCode, const string& aMessage){
    // since  m = c-k <=> k = c-m,
    //  and decode(a,b) = a-b :
    //    decode(c,m) = c-m = k
    // so the decoding can be used for calculating the key
    return decode(aCode, aMessage);
}
