#pragma once

#include <string>
#include <vector>
using std::string;

namespace codealg{

/// @brief  calculates the code  for aMessage using aKey as key (encoding)
/// if aMessage is longer than aKey, then exits while logging an error
/// @param aMessage 
/// @param aKey 
/// @return the code
string encode(const string& aMessage, const string& aKey);

/// @brief  returns the message from aCode and aKey (solving for code)
/// if aCode is longer than aKey, then exits while logging an error
/// @param aCode 
/// @param aKey 
/// @return the calcluated message
string decode(const string& aCode, const string& aKey);

/// @brief returns the key from aCode and aMessage (solving for key)
/// the returned key will be as long as the shorter parameter
/// @param aCode 
/// @param aMessage 
/// @return the calculated key
string calcKey(const string& aCode, const string& aMessage);

/// @brief returns possible keys assuming the first message starts with aMessage1Clue 
/// @param aCode1 // first code
/// @param aCode2 // second code
/// @param aMessage1Start the start of message1
/// @param maxSteps // stop after maxSteps calls
/// @param maxKeys // stop after finding maxKeys possible keys
/// @return possible keys assuming the first message starts with aMessage1Clue 
std::vector<string> findKeysWithClue(
    const string& aCode1,
    const string& aCode2,
    const string& aMessage1Start,
    long long int maxSteps,
    long long int maxKeys);


//
// helper functions to transform characters to integer-codes and back:
//

// 
// int charToInt(char c):
// c has to be lowercase alphabetic or space character
// Returns the character-index of c according to the following rules:
//   the index of an  alphabetic char is its place in the alphabet minus one(0 for a, 25 for z)
//   the index of space is 26
int charToInt(char c);

// char intToChar(int aInd):
// aInd has to be in [0,26] (inclusive) range
// intToChar is the inverse function of charToInt.
// Returns the alphabetic character or space corresponding to the character-index
char intToChar(int aInd);
}