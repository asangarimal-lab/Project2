// FacilityDatabase.h
// Purpose: Keeps accepted facilities in file order and looks them up by ID.
// Author: Asanga Rimal
//
// The vector is for display (facilities.txt order).
// The hash table is the only path used by ID search.

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

    // Reads facilities.txt. buildings holds the accepted names from buildings.txt,
    // in that file's spelling. Warnings and the load summary are printed here.
    void loadFromFile(const string& path, const vector<string>& buildings);

    // Prints every accepted facility and the total. Empty table gets a message.
    void displayAll() const;

    // Trims and uppercases the ID, then uses the hash table.
    // Reports invalid format, not found, or the facility that was found.
    void searchById(const string& rawId) const;

    int getCount() const;
    int getHashCapacity() const;
    int getHashEntries() const;
    int getHashCollisions() const;
};

#endif
