// StringUtil.cpp
// Purpose: Trim, case, and field-splitting helpers used while loading files.
// Author: Asanga Rimal

#include "StringUtil.h"

// A character we strip from the ends of a field or a typed name.
bool isTrimChar(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Drops leading and trailing whitespace. Internal spaces stay put.
string trim(const string& text) {
    int start = 0;
    int end = (int)text.size() - 1;

    while (start <= end && isTrimChar(text[start])) {
        start++;
    }
    while (end >= start && isTrimChar(text[end])) {
        end--;
    }
    if (start > end) {
        return "";
    }
    return text.substr(start, end - start + 1);
}

// Only a-z change. This is enough for the project's case rule.
string toUpper(const string& text) {
    string result = text;
    for (int i = 0; i < (int)result.size(); i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';
        }
    }
    return result;
}

// Same length and same letters, ignoring case. Spaces are compared as-is.
bool equalsIgnoreCase(const string& a, const string& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < (int)a.size(); i++) {
        char left = a[i];
        char right = b[i];
        if (left >= 'a' && left <= 'z') {
            left = left - 'a' + 'A';
        }
        if (right >= 'a' && right <= 'z') {
            right = right - 'a' + 'A';
        }
        if (left != right) {
            return false;
        }
    }
    return true;
}

// Breaks a line into fields. The last piece is still added when the line
// does not end with a pipe, including a line with no pipe at all.
vector<string> splitFields(const string& line) {
    vector<string> fields;
    string piece = "";

    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] == '|') {
            fields.push_back(trim(piece));
            piece = "";
        } else {
            piece += line[i];
        }
    }
    fields.push_back(trim(piece));
    return fields;
}

// Builds data/facilities.txt from the directory the user passed in.
string joinPath(const string& directory, const string& fileName) {
    if (directory.empty()) {
        return fileName;
    }
    char last = directory[directory.size() - 1];
    if (last == '/') {
        return directory + fileName;
    }
    return directory + "/" + fileName;
}
