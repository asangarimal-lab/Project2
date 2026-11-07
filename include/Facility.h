// Facility.h
// Purpose: One accepted facility (ID, name, building, category).
// Author: Asanga Rimal

#ifndef FACILITY_H
#define FACILITY_H

#include <string>
using namespace std;

class Facility {
private:
    string id;
    string name;
    string building;
    string category;

public:
    // Empty facility. Used only so the class has a default constructor.
    Facility();

    // ID should already be trimmed and uppercased (example: F004).
    Facility(const string& id, const string& name,
             const string& building, const string& category);

    string getId() const;
    string getName() const;
    string getBuilding() const;
    string getCategory() const;

    // Prints ID | name | building | category. Does not add a newline.
    void display() const;
};

#endif
