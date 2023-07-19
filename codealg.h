#pragma once

#include <string>
#include <vector>
using std::string;

namespace codealg{
// calculates the code aMessage using aKey as key (encoding)
// if aMessage is longer than aKey, then exits while logging an error
string encode(const string& aMessage, const string& aKey);
// returns the message from aCode and aKey (solving for code)
// if aCode is longer than aKey, then exits while logging an error
string decode(const string& aCode, const string& aKey);
// returns the key from aCode and aMessage (solving for key)
// the returned key will be as long as the shorter parameter
string calcKey(const string& aCode, const string& aMessage);


/// @brief returns possible keys assuming the first message starts with aMessage1Clue 
/// @param aCode1 // first code
/// @param aCode2 // second code
/// @param aMessage1Clue the start of message1
/// @return possible keys assuming the first message starts with aMessage1Clue 
std::vector<string> findKeysWithClue(const string& aCode1, const string& aCode2, const string& aMessage1Start);


//
// helper functions to transform characters to integer-codes:
//

// 
// int charToInt(char c):
// c has to be lowercase alphabetic or space character
// Returns the index of c according to the following rules:
//   the index of an  alphabetic char is its place in the alphabet minus one(0 for a, 25 for z)
//   the index of space is 26
int charToInt(char c);

// char intToChar(int aInd):
// aInd has to be in [0,26] (inclusive) range
// intToChar is the inverse function of charToInt.
// Returns the alphabetic character or space corresponging to the index
char intToChar(int aInd);
}