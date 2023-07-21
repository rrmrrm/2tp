#include "codealg.h"

#include "logging.h"
#include "wordTree.h"

#include <assert.h>
#include <cstdlib> // std::getenv
#include <fstream>
#include <vector>

using std::string;
using wordTree::Node;

namespace codealg{
    
int charToInt(char c){
    int ascii = (int)c;
    if(ascii == (int)' ')
        return 26;
    if(ascii < (int)'a' || ascii > (int)'z'){
        LOG_deb("error-deb: charToInt: invalid character: '" << c << "'")
        LOG_error("error: (see debug log if exists)")
        assert(false);
    }
    return ascii - (int)'a';
}

char intToChar(int aInd){
    if(aInd == 26)
        return ' ';
    if(aInd < 0 || aInd > 25){
        LOG_deb("error-deb: intToChar: invalid character character-index:" << aInd)
        LOG_error("error: (see debug log if exists)")
        assert(false);
    }
    char c = (int)'a' + aInd;
    return c;
}
string encode(const string& aMessage, const string& aKey){
    if(aMessage.size() > aKey.size()){
        LOG_deb("error-deb: encode: aMessage.size (" 
            << aMessage.size() 
            << ") was larger than key.size (" 
            << aKey.size() << ")")
        LOG_error("error: (see debug log if exists)")
        assert(false);
    }
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
    if(aCode.size() > aKey.size()){
        LOG_deb("error-deb: encode: aCode.size (" 
            << aCode.size() 
            << ") was larger than key.size (" 
            << aKey.size() << ")")
        LOG_error("error: (see debug log if exists)")
        assert(false);
    }
    string decoded = aCode;
    for(int i = 0 ; i < aCode.size() ; ++i){
        int valCode = charToInt(aCode.at(i));
        int valKey = charToInt(aKey.at(i));
        int codeVal = (valCode - valKey + 27) % 27;
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

    // if the message is shorter i shorten the code to match its length
    if(aMessage.size() < aCode.size())
        return decode(aCode.substr(0, aMessage.size()), aMessage);
    return decode(aCode, aMessage);
}


/// @brief expectedCharInd
/// @param aMsgChar message (a single character)
/// @param aCode code for aMsgChar
/// @param aOtherCode code for the other message
/// @return assuming both aCode and aOtherCode were created with the same key: 
///    returns the characterIndex of the second message
int expectedCharInd(char aMsgChar, char aCode, char aOtherCode){
    LOG_trace("ExpectedCharCode(..)..")
    LOG_indent(1)
    int msgCharInd = charToInt(aMsgChar);
    int codeInd = charToInt(aCode);
    int otherCodeInd = charToInt(aOtherCode);
    LOG_trace("aMsgChar: " << aMsgChar << ". aCode: " << aCode << ")")
    LOG_trace("aOtherCode: " << aOtherCode)
    // m2Char = m1Char + code2Char - code1Char
    int expectedCharInd = (msgCharInd + otherCodeInd - codeInd + 27 ) % 27;
    LOG_trace("expectedChar: " << intToChar(expectedCharInd))
    LOG_indent(-1)
    LOG_trace("ExpectedCharCode DONE")
    return expectedCharInd;
}

void findKeys(Node* aWordTree, Node* aContinue, Node* aOther,
        const string& aCodeContinue, const string& aCodeOther, int aMatchInd, 
        string aKeyPrefix, 
        std::vector<string>& aKeys);
void stepNodes(
        Node* aRoot,
        Node* aNode1, Node* aNode2, 
        const string& aCode1, const string& aCode2, 
        int aMatchInd,
        const string& aKeyPrefix,
        std::vector<string>& aKeys,
        bool checkSecond //marks whether we should calculate the next step fore aNode2
        ){
    for(int nextChar1Ind = 0 ; nextChar1Ind < 27 ; ++nextChar1Ind){
        char nextChar1 = intToChar(nextChar1Ind);
        Node* next1 = aNode1->nodeAtInd(nextChar1Ind);
        Node* next2 = aNode2;
        int nextCharInd2 = -1;
        if(checkSecond){
            // note: if next is null, then calculating expInd is a waste of time, but this way i have a simple clean if statement
            nextCharInd2 = expectedCharInd(nextChar1, aCode1.at(aMatchInd), aCode2.at(aMatchInd));
            next2 = aNode2->nodeAtInd(nextCharInd2);
        }
        if(next1 && (!checkSecond || next2)){
            // if charInd is 26 (' ') then a new word has to be started in the continued message
            if(nextChar1Ind == 26){
                LOG_trace("expecting a space here in the continued message. resetting next")
                next1 = aRoot;
            }
            bool resetSecondIfSteppingOnSpace = checkSecond;
            // if expInd is 26 (' ') then a new word has to be started in the other message
            if(resetSecondIfSteppingOnSpace &&  nextCharInd2 == 26){
                LOG_trace("expecting a space here in the other message. resetting otherNext")
                next2 = aRoot;
            }
            if(nextChar1Ind == 26 && nextCharInd2 == 26){
                LOG_warn("Both messages can end here. (index = " << aMatchInd << ") continuing by trying all words at the remaining part. Might take a while!")
            }
            char keyChar = calcKey(string()+aCode1.at(aMatchInd), string()+nextChar1).at(0);
            
            findKeys(aRoot,
                    next1, next2,
                    aCode1, aCode2,
                    aMatchInd+1,
                    aKeyPrefix+keyChar,
                    aKeys);
        }
    }
}

void findKeys(Node* aWordTree, Node* aContinue, Node* aOther,
        const string& aCodeContinue, const string& aCodeOther, int aMatchInd, 
        string aKeyPrefix, 
        std::vector<string>& aKeys){
    LOG_trace("findKeys START... matchInd: " << aMatchInd)
    LOG_indent(3)
    //if either essage ended, then it has to end with a full word. 
    // (if both ended the same applies, but in that case we dont recurse further, just collect the keys).
    // if either ended, then i dont iterate the aContinue or aOther, just checking if they point to the end of a full word
    if(aMatchInd >= aCodeContinue.size() && !aContinue->isWord()){
        LOG_trace("message1 ended (at aMatchInd=" << aMatchInd << "). BUT aContinue was NOT a full word, so no keys found on this path")
        LOG_trace("message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        LOG_trace("message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        LOG_trace("FindContin.. DONE")
        LOG_indent(-3)
        return;
    }
    if(aMatchInd >= aCodeOther.size() && !aOther->isWord()){
        LOG_trace("message2 ended (at aMatchInd=" << aMatchInd << "). BUT aOther was NOT a full word, so no keys found on this path")
        LOG_trace("message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        LOG_trace("message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        LOG_trace("FindContin.. DONE")
        LOG_indent(-3)
        return;
    }
    // both ended, so we dont recurse further, just store the new key
    if(aMatchInd >= aCodeContinue.size() && aMatchInd >= aCodeOther.size()){
        LOG_trace("Both messages ended (aMatchInd=" << aMatchInd << ")")
        aKeys.push_back(aKeyPrefix);

        LOG_trace("NEW msg option for message1: '" << decode(aCodeContinue, aKeyPrefix) << "'")
        LOG_trace("NEW msg option for message2: '" << decode(aCodeOther, aKeyPrefix) << "'")
        LOG_trace("NEW KEY:'" << aKeyPrefix << "'")
        LOG_trace("FindContin.. DONE")
        LOG_indent(-3)
        return;
    }
    // only first message ended
    if(aMatchInd >= aCodeContinue.size()){
        // we only need to find posible endings of message2
        LOG_trace("message1 ended (originally ended at aMatchInd=" << aMatchInd << ").")
        LOG_trace("message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        LOG_trace("message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        stepNodes(
            aWordTree,
            aOther, aContinue, // note that i switched the order of the nodes (because the stepNodes will only step its aNode1 formal parameter
            aCodeOther, aCodeContinue,
            aMatchInd,
            aKeyPrefix, aKeys,
            false // dont check end iterate first message and node
        );
        LOG_trace("FindContin.. DONE")
        LOG_indent(-3)
        return;
    }
    // only second message ended
    if(aMatchInd >= aCodeOther.size()){
        LOG_trace("message2 ended (originally ended at aMatchInd=" << aMatchInd << ").")
        LOG_trace("message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        LOG_trace("message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'")
        stepNodes(
            aWordTree,
            aContinue, aOther, 
            aCodeContinue, aCodeOther,
            aMatchInd,
            aKeyPrefix, aKeys,
            false // dont check end iterate first message and node
        );
        LOG_trace("FindContin.. DONE")
        LOG_indent(-3)
        return;
    }

    // neither message ended
    stepNodes(
        aWordTree,
        aContinue, aOther, 
        aCodeContinue, aCodeOther,
        aMatchInd,
        aKeyPrefix, aKeys,
        true // iterate both messages and nodes
    );
    LOG_trace("FindContin.. DONE")
    LOG_indent(-3)
}

std::vector<string> findKeysWithClue(const string& aCode1, const string& aCode2, const string& aMessage1Clue){
    std::vector<string> possibleKeys;
    
    int wordCnt = 0;
    Node* wordTree = new Node;
    //
    // read words from file and build word-tree.
    //
    {
    char* fileCStr = std::getenv("WORDS_TXT_FULLPATH");
    // file that contains the words:
    std::string file = "../words.txt";
    // file will be initialized from fileCstr unless it was empty. 
    if(fileCStr){
        file = std::string(fileCStr);
    }
    LOG_trace("file name:" << file)
    std::ifstream wordsStream(file);
    if(!wordsStream.is_open()){
        // try log current working dir
        if(auto pwd = std::getenv("PWD")){
            LOG_deb("error-deb: findKeysWithClue: couldn't open file with the list of the words. file name: '"
                << file << "', working directory is '" 
                << pwd << "'")
        }
        else{
            LOG_deb("error-deb: couldn't open file with the list of the words. file name: '"
                << file << "'")
        }
        LOG_error("error: couldn't open file with the list of the words. file name: '"
                << file << "'")
        assert(false);
    }
    std::string word;
    while(wordsStream >> word){
        Node* treeIter = wordTree;
        for(int i = 0 ; i < word.size() ; ++i){
            char c = word.at(i);
            Node* nextNode = treeIter->nodeAtChar(c);
            if(nextNode == nullptr){
                // next character isnt stored yet, we have to create a node for it
                Node* newNode = new Node;
                treeIter->addNodeAtChar(c, newNode);
                nextNode = treeIter->nodeAtChar(c);
            }
            // next char is now stored in the tree, step to it
            treeIter = nextNode;
        }
        treeIter->markAsWord();
        ++wordCnt;
    }
    wordsStream.close();
    }

    //
    // iterate over some characters  of the first message using aMessage1Clue and
    // follow the messages in a word-tree iterators.
    //

    //word-tree iterators:
    Node* msg1Iter = wordTree;
    Node* msg2Iter = wordTree;
    int i = 0;
    for( ; i < aMessage1Clue.size() ; ++i){
        LOG_trace("i:" << i)
        char nextChar1 = aMessage1Clue.at(i);
        // calculate the other char using the [msg1 - msg2 = code1 - code2] equality
        char nextChar2 = intToChar(expectedCharInd(nextChar1, aCode1.at(i), aCode2.at(i)));
        
        if(!msg1Iter){
            LOG_deb("error-deb: findKeysWithClue: msg1Iter was null")
            LOG_error("error: while iterating clue (see debug log if exists)")
            assert(false);
        }
        if(!msg2Iter){
            LOG_deb("error-deb: findKeysWithClue: msg2Iter was null")
            LOG_error("error: while iterating clue (see debug log if exists)")
            assert(false);
        }

        msg1Iter = msg1Iter->nodeAtChar(nextChar1);
        msg2Iter = msg2Iter->nodeAtChar(nextChar2);

        int nextChar1Ind = charToInt(nextChar1);
        int nextChar2Ind = charToInt(nextChar2);
        // if charInd is 26 (' ') then a new word has to be started in the continued message
        if(nextChar1Ind == 26){
            LOG_trace("expecting a space here in the continued message. resetting next")
            msg1Iter = wordTree;
        }
        // if expInd is 26 (' ') then a new word has to be started in the other message
        if(nextChar2Ind == 26){
            LOG_trace("expecting a space here in the other message. resetting otherNext")
            msg2Iter = wordTree;
        }
        // both words might have a space at this place, meaning we continue from the root
        if(nextChar1Ind == 26 && nextChar2Ind == 26){
            LOG_deb("Both messages can end here. (index = " << i << ") continuing by trying all words at the remaining part. Might take a while!")
        }
    }
    // note aMessage1Clue migh be longer than message2

    //
    // find possible endings
    //
    findKeys(
        wordTree, msg2Iter, msg1Iter,
        aCode2, aCode1, i,
        calcKey(aCode1, aMessage1Clue),
        possibleKeys
    );
    delete wordTree;
    return possibleKeys;
}
} // codealg
