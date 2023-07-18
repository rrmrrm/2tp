#include "codealg.h"

#include "logging.h"
#include "wordTree.h"

#include <assert.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::ostream;
using logging::TAB;
using wordTree::Node;

namespace codealg{
    
int charToInt(char c){
    int ascii = (int)c;
    if(ascii == (int)' ')
        return 26;
    assert(ascii >= (int)'a' && ascii <= (int)'z');
    return ascii - (int)'a';
}

char intToChar(int aInd){
    if(aInd == 26)
        return ' ';
    if(aInd < 0 || aInd > 25){
        std::cerr << "error: invalid character character-index:" << aInd << std::endl;
        assert(aInd >= 0 && aInd <= 25);
    }
    char c = (int)'a' + aInd;
    return c;
}
string encode(const string& aMessage, const string& aKey){
    assert(aMessage.size() <= aKey.size());
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
    assert(aCode.size() <= aKey.size());
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
    cout << TAB << "ExpectedCharCode(..).." << endl;
    TAB += 1;
    int msgCharInd = charToInt(aMsgChar);
    int codeInd = charToInt(aCode);
    int otherCodeInd = charToInt(aOtherCode);
    cout << TAB << "aMsgChar: " << aMsgChar << ". aCode: " << aCode << ")" << endl;
    cout << TAB << "aOtherCode: " << aOtherCode << endl;
    // m2Char = m1Char + code2Char - code1Char
    int expectedCharInd = (msgCharInd + otherCodeInd - codeInd + 27 ) % 27;
    cout << TAB << "expectedChar: " << intToChar(expectedCharInd) << endl;
    TAB -= 1;
    cout << TAB << "ExpectedCharCode DONE" << endl;
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
        string aKeyPrefix, 
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
                cout << TAB << "expecting a space here in the continued message. resetting next" << endl;
                next1 = aRoot;
            }
            bool resetSecondIfSteppingOnSpace = checkSecond;
            // if expInd is 26 (' ') then a new word has to be started in the other message
            if(resetSecondIfSteppingOnSpace &&  nextCharInd2 == 26){
                cout << TAB << "expecting a space here in the other message. resetting otherNext" << endl;
                next2 = aRoot;
            }
            if(nextChar1Ind == 26 && nextCharInd2 == 26){
                cout << TAB << "Both messages can end here. (index = " << aMatchInd << ") continuing by trying all words at the remaining part. Might take a while!" << endl;
            }
            char keyChar = calcKey(string()+aCode1.at(aMatchInd), string()+nextChar1).at(0);
            
            TAB += 3;
            findKeys(aRoot,
                    next1, next2,
                    aCode1, aCode2,
                    aMatchInd+1,
                    aKeyPrefix+keyChar,
                    aKeys);
            TAB -= 3;
        }
    }
}

void findKeys(Node* aWordTree, Node* aContinue, Node* aOther,
        const string& aCodeContinue, const string& aCodeOther, int aMatchInd, 
        string aKeyPrefix, 
        std::vector<string>& aKeys){
    cout << TAB << "findKeys START... matchInd: " << aMatchInd << endl;

    //if either essage ended, then it has to end with a full word. 
    // (if both ended the same applies, but in that case we dont recurse further, just collect the keys).
    // if either ended, then i dont iterate the aContinue or aOther, just checking if they point to the end of a full word
    if(aMatchInd >= aCodeContinue.size() && !aContinue->isWord()){
        cout << TAB << "message1 ended (at aMatchInd=" << aMatchInd << "). BUT aContinue was NOT a full word, so no keys found on this path" << endl;
        cout << TAB << "message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        cout << TAB << "message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        return;
    }
    if(aMatchInd >= aCodeOther.size() && !aOther->isWord()){
        cout << TAB << "message2 ended (at aMatchInd=" << aMatchInd << "). BUT aOther was NOT a full word, so no keys found on this path" << endl;
        cout << TAB << "message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        cout << TAB << "message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        return;
    }
    // both ended, so we dont recurse further, just store the new key
    if(aMatchInd >= aCodeContinue.size() && aMatchInd >= aCodeOther.size()){
        cout << TAB << "Both messages ended (aMatchInd=" << aMatchInd << ")" << endl;
        aKeys.push_back(aKeyPrefix);

        cout << TAB << "NEW msg option for message1: '" << decode(aCodeContinue, aKeyPrefix) << "'" << endl;
        cout << TAB << "NEW msg option for message2: '" << decode(aCodeOther, aKeyPrefix) << "'" << endl;
        cout << TAB << "NEW KEY:'" << aKeyPrefix << "'" << endl;
        return;
    }
    // only first message ended
    if(aMatchInd >= aCodeContinue.size()){
        // we only need to find posible endings of message2
        cout << TAB << "message1 ended (originally ended at aMatchInd=" << aMatchInd << ")." << endl;
        cout << TAB << "message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        cout << TAB << "message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        stepNodes(
            aWordTree,
            aOther, aContinue, // note that i switched the order of the nodes (because the stepNodes will only step its aNode1 formal parameter
            aCodeOther, aCodeContinue,
            aMatchInd,
            aKeyPrefix, aKeys,
            false // dont check end iterate first message and node
        );
        cout << TAB << "FindContin.. DONE" << endl;
        return;
    }
    // only second message ended
    if(aMatchInd >= aCodeOther.size()){
        cout << TAB << "message2 ended (originally ended at aMatchInd=" << aMatchInd << ")." << endl;
        cout << TAB << "message1 solution prefix: '" << decode(aCodeContinue.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        cout << TAB << "message2 solution prefix: '" << decode(aCodeOther.substr(0, aKeyPrefix.size()), aKeyPrefix) << "'" << endl;
        stepNodes(
            aWordTree,
            aContinue, aOther, 
            aCodeContinue, aCodeOther,
            aMatchInd,
            aKeyPrefix, aKeys,
            false // dont check end iterate first message and node
        );
        cout << TAB << "FindContin.. DONE" << endl;
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
    assert(fileCStr);
    std::string file = std::string(fileCStr);
    std::cout << TAB << "file name:" << file << std::endl;
    std::ifstream wordsStream(file);
    assert(wordsStream.is_open());
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
        cout << TAB << "i:" << i << endl;
        char nextChar1 = aMessage1Clue.at(i);
        // calculate the other char using the [msg1 - msg2 = code1 - code2] equality
        char nextChar2 = intToChar(expectedCharInd(nextChar1, aCode1.at(i), aCode2.at(i)));
        msg1Iter = msg1Iter->nodeAtChar(nextChar1);
        msg2Iter = msg2Iter->nodeAtChar(nextChar2);
        assert(msg1Iter);
        assert(msg2Iter);

        int nextChar1Ind = charToInt(nextChar1);
        int nextCharInd2 = -1;
        // if charInd is 26 (' ') then a new word has to be started in the continued message
        if(nextChar1Ind == 26){
            cout << TAB << "expecting a space here in the continued message. resetting next" << endl;
            msg1Iter = wordTree;
        }
        // if expInd is 26 (' ') then a new word has to be started in the other message
        if(nextCharInd2 == 26){
            cout << TAB << "expecting a space here in the other message. resetting otherNext" << endl;
            msg2Iter = wordTree;
        }
        // both words might have a space at this place, meaning we continue from the root
        if(nextChar1Ind == 26 && nextCharInd2 == 26){
            cout << TAB << "Both messages can end here. (index = " << i << ") continuing by trying all words at the remaining part. Might take a while!" << endl;
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

    return possibleKeys;
}
} // codealg
