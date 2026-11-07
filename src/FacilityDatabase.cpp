// FacilityDatabase.cpp
// Purpose: Load, list, and ID-search facilities. ID search goes through the hash table.
// Author: Asanga Rimal

#include "FacilityDatabase.h"
#include "StringUtil.h"
#include <fstream>
#include <iostream>
using namespace std;

// True for F followed by exactly three digits. The ID must already be uppercase.
static bool isValidId(const string& id) {
    if (id.size() != 4) {
        return false;
    }
    if (id[0] != 'F') {
        return false;
    }
    for (int i = 1; i <= 3; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}

// Finds the buildings.txt spelling for this name, or "" if it was never accepted.
static string canonicalBuilding(const vector<string>& buildings, const string& name) {
    for (int i = 0; i < (int)buildings.size(); i++) {
        if (equalsIgnoreCase(buildings[i], name)) {
            return buildings[i];
        }
    }
    return "";
}

FacilityDatabase::FacilityDatabase() {
}

// Validates one file. Check order matches the project notes:
// field count, empty fields, ID format, unknown building, then duplicate.
void FacilityDatabase::loadFromFile(const string& path, const vector<string>& buildings) {
    string label = "facilities.txt";
    ifstream input(path.c_str());
    int accepted = 0;
    int rejected = 0;

    if (!input) {
        cout << "Error: cannot open file " << label << endl;
        cout << label << ": 0 accepted, 0 rejected (hash capacity "
             << table.getCapacity() << ", entries " << table.getEntryCount()
             << ", collisions " << table.getCollisionCount() << ")" << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    while (getline(input, line)) {
        lineNumber++;
        string trimmed = trim(line);
        if (trimmed.empty()) {
            continue;
        }
        // A comment is only a line whose first real character is #.
        if (trimmed[0] == '#') {
            continue;
        }

        vector<string> fields = splitFields(line);
        if (fields.size() != 4) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": expected 4 fields separated by |" << endl;
            rejected++;
            continue;
        }

        if (fields[0].empty() || fields[1].empty() ||
            fields[2].empty() || fields[3].empty()) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": empty field" << endl;
            rejected++;
            continue;
        }

        string id = toUpper(fields[0]);
        if (!isValidId(id)) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": invalid ID format, expected F followed by exactly three digits"
                 << endl;
            rejected++;
            continue;
        }

        string buildingName = canonicalBuilding(buildings, fields[2]);
        if (buildingName.empty()) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": unknown building " << fields[2] << endl;
            rejected++;
            continue;
        }

        // Name and category keep the spelling from this line.
        // The building uses the spelling from buildings.txt.
        Facility facility(id, fields[1], buildingName, fields[3]);
        if (!table.insert(facility)) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": duplicate facility ID " << id << endl;
            rejected++;
            continue;
        }

        facilities.push_back(facility);
        accepted++;
    }

    cout << label << ": " << accepted << " accepted, " << rejected
         << " rejected (hash capacity " << table.getCapacity()
         << ", entries " << table.getEntryCount()
         << ", collisions " << table.getCollisionCount() << ")" << endl;
}

// File order, not hash-bucket order.
void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        cout << "No facilities are loaded." << endl;
        return;
    }

    for (int i = 0; i < (int)facilities.size(); i++) {
        facilities[i].display();
        cout << endl;
    }
    cout << "Total facilities: " << facilities.size() << endl;
}

// Three outcomes: bad format, valid but missing, or found.
// lookup uses the table. This function does not scan the vector.
void FacilityDatabase::searchById(const string& rawId) const {
    string id = toUpper(trim(rawId));
    if (!isValidId(id)) {
        cout << "Invalid format: an ID is F followed by exactly three digits." << endl;
        return;
    }

    const Facility* found = table.lookup(id);
    if (found == nullptr) {
        cout << "Not found: no facility with ID " << id << endl;
        return;
    }

    cout << "Found: ";
    found->display();
    cout << endl;
}

int FacilityDatabase::getCount() const {
    return (int)facilities.size();
}

int FacilityDatabase::getHashCapacity() const {
    return table.getCapacity();
}

int FacilityDatabase::getHashEntries() const {
    return table.getEntryCount();
}

int FacilityDatabase::getHashCollisions() const {
    return table.getCollisionCount();
}
