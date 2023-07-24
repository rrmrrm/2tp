
#include "codealg.h"
#include "logging.h"
#include <iostream> //std::endl
#include <cstdlib> // std::getenv
#include <string>
#include <vector>

using std::string;

using namespace codealg;
int main(){
    //
    // a usecase for the codealg module (the codealg namespace)

    // key is the original key. 
    // i call it original because the code breaking algorithm will usually find multiple possible keys, not just this key.
    // The below original messages(msg1, msg2) will be encoded using this key to create code1, code2
    string key =  "aaaaaaaaaaaaaaaaaaaaaaaaaaa";
    // original messages.
    // the alagorithm finds keys different from the original, so decoding code1, code2 with such a key
    // will usually result in different messages then these original messages.
    string msg1 = "early bird catches the worm";
    string msg2 = "curiosity killed the cat";
    // code the msg1 and msg2 with the same key. 
    // the code breaking lagorithm(findKeysWithClue method ) will only know about these codes, and the first few characters from msg1(clue_msg1):
    string code1 = encode(msg1, key);
    string code2 = encode(msg2, key);

    /// note: theres is a chance the  algorithm doesn't find the original key(and thus not find the original messages), 
    /// because of memory and runtime problems: 
    ///   It finds keys and STRORES them in a container
    ///   (instead of forwarding the keys and discarding immedaitely after finding them).
    ///   Also in the worst case the number of possible keys is an exponential function of the codes' length.

    // give a clue to the algorithm: the first message starts with this string
    // can be a word but also multiple words or a partial word as long as msg1 actually starts with it 
    string clue_msg1 = "early ";
    LOG_info("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    // find possiblle keys
    auto keys = findKeysWithClue(
        code1, // code of first message 
        code2, // code of second message
        clue_msg1, // first few characters of first message
        100000, // stop after too many iterations (serves as a time limit)
        100000 // stop after finding too many keys (serves as a memory limit)
        );



    LOG_info("Possible keys and messages:")
    for(auto key : keys){
        LOG_info("Key: '" << key << "'")
        LOG_info("message 1 '" << decode(code1, key) << "'")
        LOG_info("message 2 '" << decode(code2, key) << "'")
        LOG_info("")
    }
    return 0;
}
