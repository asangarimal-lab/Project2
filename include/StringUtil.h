// StringUtil.h
// Purpose: Small string helpers shared by file loading and name matching.
// Author: Asanga Rimal

#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>
using namespace std;

// Spaces, tabs, and carriage returns. Used so Windows files load cleanly.
bool isTrimChar(char c);

// Removes whitespace from both ends. Spaces inside the text stay as they are.
string trim(const string& text);

// Uppercases ASCII letters only. Digits and other characters stay the same.
string toUpper(const string& text);

// True when the two strings match if we ignore letter case.
// "Student Union" and "Student  Union" are not equal, because the spaces differ.
bool equalsIgnoreCase(const string& a, const string& b);

// Splits a data-file line on '|'. Each field is trimmed.
// Empty fields are kept so we can reject them.
vector<string> splitFields(const string& line);

// Joins a directory and a file name. Does not use an absolute path.
string joinPath(const string& directory, const string& fileName);

#endif
