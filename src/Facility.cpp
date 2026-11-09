// Facility.cpp
// Purpose: Stores and prints one facility record.
// Author: Asanga Rimal

#include "Facility.h"
#include <iostream>
using namespace std;

Facility::Facility() {
    id = "";
    name = "";
    building = "";
    category = "";
}

Facility::Facility(const string& id, const string& name,
                   const string& building, const string& category) {
    this->id = id;
    this->name = name;
    this->building = building;
    this->category = category;
}

string Facility::getId() const {
    return id;
}

string Facility::getName() const {
    return name;
}

string Facility::getBuilding() const {
    return building;
}

string Facility::getCategory() const {
    return category;
}

// Four fields, same order as the file: ID, name, building, category.
void Facility::display() const {
    cout << id << " | " << name << " | " << building << " | " << category;
}
