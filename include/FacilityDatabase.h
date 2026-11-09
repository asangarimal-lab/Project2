// FacilityDatabase.h
// Purpose: Holds the facilities and looks them up by ID.
// Author: Asanga Rimal
//
// I keep a vector so "view all" stays in facilities.txt order.
// The hash table is only for ID search. Searching does not walk the vector.

#ifndef FACILITYDATABASE_H
#define FACILITYDATABASE_H

#include <string>
#include <vector>
#include "Facility.h"
#include "HashTable.h"
using namespace std;

class FacilityDatabase {
private:
    vector<Facility> facilities;
    HashTable table;

public:
    FacilityDatabase();

    // Reads facilities.txt. buildings is the accepted names, spelled the
    // way buildings.txt had them. Prints a warning for each bad line.
    void loadFromFile(const string& path, const vector<string>& buildings);

    // Prints the accepted facilities and the total. Says so if the list is empty.
    void displayAll() const;

    // Trims the typed ID, uppercases it, then asks the hash table.
    void searchById(const string& rawId) const;

    int getCount() const;
    int getHashCapacity() const;
    int getHashEntries() const;
    int getHashCollisions() const;
};

#endif
