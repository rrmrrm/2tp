#include "codealg.h"
#include <gtest/gtest.h>

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
