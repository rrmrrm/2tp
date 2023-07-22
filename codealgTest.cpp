#include "codealg.h"
#include "logging.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>

using namespace codealg;
TEST(shouldpass, shouldpass){
    EXPECT_TRUE(true);
}
//
// tests for encode:
//
TEST(CodeAlg, encode1char_case1){
    string message = "a";
    string key = " ";
    string code = encode(message, key);
    EXPECT_EQ(code , " ");
}
TEST(CodeAlg, encode1char_case2){
    string message = "b";
    string key = "c";
    string code = encode(message, key);
    EXPECT_EQ(code , "d");
}
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    int keyLen = std::max(msg1.size(), msg2.size());
    string subKey = key.substr(0, keyLen);
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    string subKey = key.substr(0, msg2.size());
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    int keyLen = std::max(msg1.size(), msg2.size());
    string subKey = key.substr(0, keyLen);
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}

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
    auto keys = findKeysWithClue(code1, code2, clue_msg1);
    int keyLen = std::max(msg1.size(), msg2.size());
    string subKey = key.substr(0, keyLen);
    auto foundIt = std::find(keys.begin(), keys.end(), subKey);
    // key shouldn't overiterate keys
    ASSERT_NE(foundIt, keys.end());
    // make sure the found key really is the original key
    EXPECT_EQ(*foundIt, subKey);
}
/// TODO: generated tests
/// TODO: keys  only encode valid messages(words from the file) and they also encode finds keyLen long part of the key