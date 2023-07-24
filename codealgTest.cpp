#include "codealg.h"
#include "logging.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib> // std::getenv
#include <stdlib.h> // srand, rand
#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace codealg;
TEST(shouldpass, shouldpass){
    EXPECT_TRUE(true);
}
//
// tests for encode:
//

/// @brief  encode('a' + ' ') should yield ' ' since the character-index of 'a' is 0
TEST(CodeAlg, encode1char_case1){
    string message = "a";
    string key = " ";
    string code = encode(message, key);
    EXPECT_EQ(code , " ");
}
/// @brief 'b'+'c' ~ 1+2 = 3 ~ 'd'
TEST(CodeAlg, encode1char_case2){
    string message = "b";
    string key = "c";
    string code = encode(message, key);
    EXPECT_EQ(code , "d");
}
/// @brief ' ' + ' ' ~ 26+26 = 27-1-1=25 ~ 'z'  (mod27)
TEST(CodeAlg, encode1char_case3){
    string message = " ";
    string key = " ";
    string code = encode(message, key);
    EXPECT_EQ(code , "z");
}
TEST(CodeAlg, encode){
    string message = "helloworld";
    string key = "abcdefgijk";
    string code = encode(message, key);
    EXPECT_EQ(code , "hfnosauzun");
}
/// @brief 
/// given: the key is longer than the message 
/// when: calling encode(message, key)
/// then: the resulting code should be the same as encode(message, key.substr(0,len(msg)))
TEST(CodeAlg, encodeLongKey){
    string message = "helloworld";
    string key = "abcdefgijkLONGKEY";
    string code = encode(message, key);
    EXPECT_EQ(code , "hfnosauzun");
}

//
// tests for decode
// (created by switching around the texts in "encode" testcases):
//
TEST(CodeAlg, decode1char_case1){
    string code = " ";
    string key = " ";
    string message = decode(code, key);
    EXPECT_EQ(message , "a");
}
TEST(CodeAlg, decode1char_case2){
    string code = "d";
    string key = "c";
    string message = decode(code, key);
    EXPECT_EQ(message , "b");
}
TEST(CodeAlg, decode1char_case3){
    string code = "z";
    string key = " ";
    string message = decode(code, key);
    EXPECT_EQ(message , " ");
}
TEST(CodeAlg, decode){
    string code = "hfnosauzun";
    string key = "abcdefgijk";
    string message = decode(code, key);
    EXPECT_EQ(message , "helloworld");
}
TEST(CodeAlg, decodeLongKey){
    string code = "hfnosauzun";
    string key = "abcdefgijkLONGKEY";
    string message = decode(code, key);
    EXPECT_EQ(message , "helloworld");
}

//
// tests for calcKey
// (created by switching around the texts in "decode" testcases):
//
TEST(CodeAlg, calcKey1char_case1){
    string code = " ";
    string message = " ";
    string key = calcKey(code, message);
    EXPECT_EQ(key , "a");
}
TEST(CodeAlg, calcKey1char_case2){
    string code = "d";
    string message = "c";
    string key = calcKey(code, message);
    EXPECT_EQ(key , "b");
}
TEST(CodeAlg, calcKey1char_case3){
    string code = "z";
    string message = " ";
    string key = calcKey(code, message);
    EXPECT_EQ(key , " ");
}
TEST(CodeAlg, calcKey){
    string code = "hfnosauzun";
    string message = "abcdefgijk";
    string key = calcKey(code, message);
    EXPECT_EQ(key , "helloworld");
}
TEST(CodeAlg, calcKeyLongMessage){
    string code = "hfnosauzun";
    string message = "abcdefgijkLONGKEY";
    string key = calcKey(code, message);
    EXPECT_EQ(key , "helloworld");
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given messages with equal length
TEST(CodeAlg, findKeysWithClue_msg_len_eq){
    string key =  "no stain yet on";
    string clue_msg1 =
                  "i am";
    string msg1 = "i am error";
    string code1 = encode(msg1, key);
    string msg2 = "welcome to";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given the clue is short
TEST(CodeAlg, findKeysWithClue_clue_short){
    string key =  "the moon was seen";
    string clue_msg1 =
                  "i";
    string msg1 = "i am error";
    string code1 = encode(msg1, key);
    string msg2 = "welcome to diet";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}
/// @brief
/// test wheter findKeysWithClue finds the original key
/// given the clue is empty
TEST(CodeAlg, findKeysWithClue_clue_empty){
    string key =  "no words were laid";
    string clue_msg1 =
                  "";
    string msg1 = "i am error";
    string code1 = encode(msg1, key);
    string msg2 = "the cake is a lie";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given the clue and first message and second message have the same length
TEST(CodeAlg, findKeysWithClue_clue_same_length){
    string key =  " on stream or stone ";
    string clue_msg1 =
                  "video game reference";
    string msg1 = "video game reference";
    string code1 = encode(msg1, key);
    string msg2 = "also a bad reference";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given the clue and message1 have the same length but message2 is longer
TEST(CodeAlg, findKeysWithClue_clue_long){
    string key =  "when durin woke and walked alone he";
    string clue_msg1 =
                  "also a bad video game reference";
    string msg1 = "also a bad video game reference";
    string code1 = encode(msg1, key);
    string msg2 = "another a bad video game reference";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given: both messages have a space at the same place
TEST(CodeAlg, findKeysWithClue_both_space_after_clue){
    string key =  "named the nameless hills and dells";
    string clue_msg1 =
                  "proper";
    string msg1 = "proper okay organic print";
    string code1 = encode(msg1, key);
    string msg2 = "suspect hat investigate this";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given long messages
TEST(CodeAlg, findKeysWithClue_msg_long){
    string key =  "the world was young the mountains green the world was young the mountains green the world was young the mountains green";
    string clue_msg1 =
                  "pressure ";
    string msg1 = "pressure general gas form except captain price call of duty western wall ability high all other painful";
    string code1 = encode(msg1, key);
    string msg2 = "pc payment open could beautiful settlement worried so long and thanks for all the fish horizon index";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    int keyLen = std::max(msg1.size(), msg2.size());
    string subKey = key.substr(0, keyLen);
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief
/// test wheter findKeysWithClue finds the original key
/// given messages with different size
TEST(CodeAlg, findKeysWithClue_msg_len_differs1){
    string key =  "the world was young the mountains green";
    string clue_msg1 =
                  "all y";
    string msg1 = "all yours base belong to us";
    string code1 = encode(msg1, key);
    string msg2 = "the cake is a lie a winner is you";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}
/// @brief
/// test wheter findKeysWithClue finds the original key
/// given messages with different size
TEST(CodeAlg, findKeysWithClue_msg_len_differs2){
    string key =  "the world was young the mountains green";
    string clue_msg1 =
                  "the cake ";
    string msg1 = "the cake is a lie a winner is you";
    string code1 = encode(msg1, key);
    string msg2 = "all yours base belong to us";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    int keyLen = std::max(msg1.size(), msg2.size());
    string subKey = key.substr(0, keyLen);
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// TODO: this test is still failing. 
/// it seems the original key is not in the first 10000000 found keys
/// @brief
/// test wheter findKeysWithClue finds the original key
/// given long messages size
TEST(CodeAlg, findKeysWithClue_msg_len_differs3){
    string key =  "the world was young the mountains green";
    string clue_msg1 =
                  "all y";
    string msg1 = "all your base belong to us";
    string code1 = encode(msg1, key);
    string msg2 = "a winner is you";
    string code2 = encode(msg2, key);
    LOG_deb("Original codes and parameters:" << std::endl
            << "key  '" << key << "'" << std::endl
            << "msg1: '" << msg1 << "'" << std::endl
            << "code1:'" << code1 << "'" << std::endl
            << "msg2: '" << msg2 << "'" << std::endl
            << "code2: '" << code2 << "'" << std::endl
            << "clue for msg1: '" << clue_msg1 << "'" << std::endl)
    auto keys = findKeysWithClue(code1, code2, clue_msg1, 10000000, 1000000);
    int keyLen = std::max(msg1.size(), msg2.size());
    string subKey = key.substr(0, keyLen);
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // foundIt shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

/// @brief 
/// helper for the randomized test
/// generates a random aLen long string with a-z and space characters
/// @param aLen 
/// @return 
string randStr(int aLen){
    string ret;
    for(int i = 0 ; i < aLen ; ++i){
        ret.push_back(intToChar(rand()%27));
    }
    return ret;
}

/// @brief 
/// helper for the randomized test
/// generates a string that's around aMsgBallpark long and consists of elements of aWords
/// the generated string might be longer than aMsgBallpark
/// @param aMsgBallpark 
/// @param aWords 
/// @return 
string genMessage(int aMsgBallpark, std::vector<string> aWords){
    int len = 0;
    string message;
    bool firstWord = true;
    while(len < aMsgBallpark){
        // insert space between words
        if(firstWord){
            firstWord = false;
        }
        else{
            message.push_back(' ');
            len += 1;
        }

        int ind = rand() % aWords.size();
        message += aWords.at(ind);
        len += aWords.at(ind).size();
    }
    return message;
}
/// @brief generates randomized tests.
/// in each test:
///   generates a message pair(using words list file) and a clue(a starting substring of the fist message ),
///    and a key,
///   and uses these to create code-pairs.
///   Then runs the codealg::findKeysWithClue to create a list of keys that might have been used to create the code-pair
///   Then tests if decoding the codes with the reuturned keys results in valid messages(concatenation of english words)
///   Also checks if the first message starts with the given message clue.
///
///   does NOT test if the original key was also found, but it produces a warning otherwise("warning: findKeysWithClue didn't find the original key")
///  
TEST(CodeAlg, findKeysWithClue_generated_good_keys_only){
    //
    // read the words from a file into containers
    //
    std::unordered_set<string> words; // for checking if a string is a valid word
    std::vector<string> wordsVector; // for generating syntactically correct messages
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
            LOG_deb("error-deb: TEST(CodeAlg, findKeysWithClue_generated_good_keys_only: couldn't open file with the list of the words. file name: '"
                << file << "', working directory is '" 
                << pwd << "'")
        }
        else{
            LOG_deb("error-deb: TEST(CodeAlg, findKeysWithClue_generated_good_keys_only: couldn't open file with the list of the words. file name: '"
                << file << "'")
        }
        LOG_error("error: TEST(CodeAlg, findKeysWithClue_generated_good_keys_only: couldn't open file with the list of the words. file name: '"
                << file << "'")
        assert(false);
    }
    std::string word;
    while(wordsStream >> word){
        words.insert(word);
        wordsVector.push_back(word);
    }
    wordsStream.close();
    }

    //
    // generate key, code1, code2 triplets and test findKeysWithClue on them
    //
    srand(0);
    /// number of generated testcases:
    int tests = 100;
    /// upper limit for generated messages
    int maxMsgLen = 1000;

    for(int i = 0 ; i < tests ; ++i){
        LOG_deb("TEST(CodeAlg, findKeysWithClue_generated_good_keys_only):" << i << ". test")
        // the generated message1 will have around msg1Ballpark characters in it
        //  Note: actual messageLength might differ by the longest word's length
        int msg1Ballpark = rand() % maxMsgLen;
        int msg2Ballpark = rand() % maxMsgLen;
        string msg1 = genMessage(msg1Ballpark, wordsVector);
        string msg2 = genMessage(msg2Ballpark, wordsVector);
        int msg1Len = msg1.size();
        int msg2Len = msg2.size();
        int keyLen = std::max(msg1Len, msg2Len) + (rand()%100); // key can be longer than both messages
        string key =  randStr(keyLen);
        int clueLen = rand() % (msg1Len+1); // clueLen is in [0, msg1Len] interval
        string clue_msg1 = msg1.substr(0, clueLen);
        string code1 = encode(msg1, key);
        string code2 = encode(msg2, key);
        int maxSteps = rand() % 100000;
        int maxKeys =  rand() % 100000;
        LOG_deb("Original codes and parameters:" << std::endl
                << "key  '" << key << "'" << std::endl
                << "msg1: '" << msg1 << "'" << std::endl
                << "code1:'" << code1 << "'" << std::endl
                << "msg2: '" << msg2 << "'" << std::endl
                << "code2: '" << code2 << "'" << std::endl
                << "clue for msg1: '" << clue_msg1 << "'" << std::endl
                << "maxSteps: '" << maxSteps << "'" << std::endl
                << "maxKeys: '" << maxKeys << "'" << std::endl)
        auto keys = findKeysWithClue(code1, code2, clue_msg1, maxSteps, maxKeys);
        LOG_deb("found: " << keys.size() << " keys" << std::endl)

        // since at this stage findKeysWithClue usually DOESN't finds the original key, 
        // i dont test for that, just put a warning here. so it shows up but doesnt hide any other failures
        int keyRelevantLen = std::max(msg1.size(), msg2.size());
        string subKey = key.substr(0, keyRelevantLen);
        auto foundIt = std::find(keys.begin(), keys.end(), subKey);
        if(foundIt == keys.end()){
            LOG_warn("warning: findKeysWithClue didn't find the original key")
        }
        // test if encoding with found keys yields sentence-pairs(message-pairs) where
        // the first message starts with msg1Clue, and syntactically correct,
        for(const auto& keyIt: keys){
            auto decoded1 = decode(code1, keyIt);
            auto decoded2 = decode(code2, keyIt);
            // test whether first message starts with the message clue
            EXPECT_EQ(decoded1.substr(0, clueLen), clue_msg1);
            // test syntactical correctness of decoded first message:
            if(decoded1.size()){
                // splitDecoded is decoded1 split into words
                std::vector<string> splitDecoded;
                std::string wordStump;
                for(char c : decoded1){
                    if(c == ' '){
                        splitDecoded.push_back(wordStump);
                        wordStump = "";
                    }
                    else{
                        wordStump.push_back(c);
                    }
                }
                // insert last wordStump
                splitDecoded.push_back(wordStump);
                for(const string& splitDecodedIt: splitDecoded){
                    bool wordValid = words.count(splitDecodedIt);
                    EXPECT_TRUE(wordValid);
                }
                
            }
            else{
                LOG_warn("warning!: TEST(CodeAlg, findKeysWithClue_generated_good_keys_only): decoded1 was empty. wont do syntactic check");
            }
            // test syntactical correctness of decoded second message:
            if(decoded2.size()){
                // splitDecoded is decoded1 split into words
                std::vector<string> splitDecoded;
                std::string wordStump;
                for(char c : decoded2){
                    if(c == ' '){
                        splitDecoded.push_back(wordStump);
                        wordStump = "";
                    }
                    else{
                        wordStump.push_back(c);
                    }
                }
                // insert last wordStump
                splitDecoded.push_back(wordStump);
                for(const string& splitDecodedIt: splitDecoded){
                    bool wordValid = words.count(splitDecodedIt);
                    EXPECT_TRUE(wordValid);
                }
            }
            else{
                LOG_warn("warning!: TEST(CodeAlg, findKeysWithClue_generated_good_keys_only): decoded2 was empty. wont do syntactic check");
            }
            
        }
    }
}
