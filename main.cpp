
#include "codealg.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

using namespace codealg;
int main(){
    string key =  "aaaaaaaaaaaaaaaaaaaaaaaaaaa";
    string msg1 = "early bird catches the worm";
    string code1 = encode(msg1, key);
    string msg2 = "curiosity killed the cat";
    string code2 = encode(msg2, key);
    cout << "running findShareKeys.." << endl;
    auto keys = findKeysWithClue(code1, code2, "early ");
    cout << "Possible keys and messages:" << endl;
    for(auto key : keys){
        cout << "Key: '" << key << "'" << endl;
        cout << "message 1 '" << decode(code1, key) << "'" << endl;
        cout << "message 2 '" << decode(code2, key) << "'" << endl;
        cout << endl;
    }
    return 0;
}
