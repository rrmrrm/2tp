
#include "codealg.h"
#include "logging.h"
#include <iostream> //std::endl
#include <cstdlib> // std::getenv
#include <string>
#include <vector>

using std::string;

using namespace codealg;
int main(){
    string key =  "aaaaaaaaaaaaaaaaaaaaaaaaaaa";
    string msg1 = "early bird catches the worm";
    string code1 = encode(msg1, key);
    string msg2 = "curiosity killed the cat";
    string code2 = encode(msg2, key);
    string clue_msg1 = "early ";
    LOG_info("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1);



    LOG_info("Possible keys and messages:")
    for(auto key : keys){
        LOG_info("Key: '" << key << "'")
        LOG_info("message 1 '" << decode(code1, key) << "'")
        LOG_info("message 2 '" << decode(code2, key) << "'")
        LOG_info("")
    }
    return 0;
}
