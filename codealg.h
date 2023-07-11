#pragma once

#include <string>
using std::string;

int charToInt(char c);
string encode(const string& aMessage, const string& aKey);
string decode(const string& aCode, const string& aKey);
