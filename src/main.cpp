// main.cpp
// Purpose: Load the data files and run the Milestone 1 menu.
// Author: Asanga Rimal
//
// This branch covers facilities and the ID hash table.
// Building names are loaded here because a facility has to name an accepted
// building. Connections and the department tree are Sebastian's and Anugrah's
// modules, so those menu options are not wired up yet.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "FacilityDatabase.h"
#include "StringUtil.h"
#include "DepartmentTree.h"
using namespace std;

// Reads buildings.txt into names, using the spelling of the first good line.
// Later lines that match ignoring case are rejected.
void loadBuildings(const string& path, vector<string>& buildings) {
    string label = "buildings.txt";
    ifstream input(path.c_str());
    int accepted = 0;
    int rejected = 0;

    if (!input) {
        cout << "Error: cannot open file " << label << endl;
        cout << label << ": 0 accepted, 0 rejected" << endl;
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
        if (trimmed[0] == '#') {
            continue;
        }

        vector<string> fields = splitFields(line);
        if (fields.size() != 1) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": expected 1 field (a building name)" << endl;
            rejected++;
            continue;
        }
        if (fields[0].empty()) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": empty building name" << endl;
            rejected++;
            continue;
        }

        bool duplicate = false;
        for (int i = 0; i < (int)buildings.size(); i++) {
            if (equalsIgnoreCase(buildings[i], fields[0])) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": duplicate building name " << fields[0] << endl;
            rejected++;
            continue;
        }

        buildings.push_back(fields[0]);
        accepted++;
    }

    cout << label << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}

void printMenu() {
    cout << endl;
    cout << "Smart Campus Menu" << endl;
    cout << "1. View all facilities" << endl;
    cout << "2. Search facility by ID" << endl;
    cout << "3. View campus graph" << endl;
    cout << "4. Display graph counts and degrees" << endl;
    cout << "5. Find reachable buildings (BFS)" << endl;
    cout << "6. Add department" << endl;
    cout << "7. Search department" << endl;
    cout << "8. Display department hierarchy" << endl;
    cout << "9. Run preorder traversal" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter choice: ";
}

// Reads a menu line ourselves so a word like "abc" does not break cin.
// Returns false when the line is blank or not a plain number.
bool readChoice(const string& raw, int& choice) {
    string text = trim(raw);
    if (text.empty()) {
        return false;
    }

    int value = 0;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }
        value = value * 10 + (text[i] - '0');
        // Menu choices are 0 through 9. Anything bigger is just invalid.
        if (value > 9) {
            return false;
        }
    }
    choice = value;
    return true;
}

void searchFacilityById(const FacilityDatabase& facilities) {
    cout << "Enter facility ID: ";
    string rawId;
    if (!getline(cin, rawId)) {
        return;
    }
    if (trim(rawId).empty()) {
        cout << "Error: facility ID cannot be blank." << endl;
        return;
    }
    facilities.searchById(rawId);
}
void addDepartment(DepartmentTree& departments) {
    cout << "Enter department name: ";

    string name;
    if (!getline(cin, name)) {
        return;
    }

    if (trim(name).empty()) {
        cout << "Error: department name cannot be blank." << endl;
        return;
    }

    string parentName = "NONE";

    if (!departments.isEmpty()) {
        cout << "Enter parent department name: ";

        if (!getline(cin, parentName)) {
            return;
        }
    }

    string error;

    if (departments.addDepartment(name, parentName, error)) {
        cout << "Department added: " << trim(name) << endl;
    } else {
        cout << "Error: " << error << "." << endl;
    }
}

void searchDepartment(const DepartmentTree& departments) {
    if (departments.isEmpty()) {
        cout << "Department tree is empty." << endl;
        return;
    }

    cout << "Enter department name: ";

    string name;
    if (!getline(cin, name)) {
        return;
    }

    departments.searchDepartment(name);
}

int main(int argc, char* argv[]) {
    // Default folder is data/. Example: ./campus testdata/collision
    string dataDir = "data";
    if (argc >= 2) {
        dataDir = argv[1];
    }

    vector<string> buildings;
    FacilityDatabase facilities;
    DepartmentTree departments;

    // Required load order starts with buildings, then connections,
    // then facilities, then departments. Connections and departments
    // are loaded by the other modules when those branches are merged.
    loadBuildings(joinPath(dataDir, "buildings.txt"), buildings);

    // Sebastian: load connections into CampusGraph here.

    facilities.loadFromFile(joinPath(dataDir, "facilities.txt"), buildings);

departments.loadFromFile(joinPath(dataDir, "departments.txt"));

bool running = true;
    while (running) {
        printMenu();
        string line;
        if (!getline(cin, line)) {
            break;
        }

        int choice = -1;
        if (!readChoice(line, choice)) {
            cout << "Invalid menu choice. Enter a number from the menu." << endl;
            continue;
        }

        if (choice == 1) {
            facilities.displayAll();
        } else if (choice == 2) {
            searchFacilityById(facilities);
        } else if (choice == 3 || choice == 4 || choice == 5) {
            cout << "Campus graph module is not available yet." << endl;
        } else if (choice == 6) {
            addDepartment(departments);
        } else if (choice == 7) {
            searchDepartment(departments);
        } else if (choice == 8) {
            departments.displayHierarchy();
        } else if (choice == 9) {
            departments.displayPreorder();
        } else if (choice == 0) {
            running = false;
        } else {
            cout << "Invalid menu choice. Enter a number from the menu." << endl;
        }
    }

    return 0;
}
