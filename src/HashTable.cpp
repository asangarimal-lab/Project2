// HashTable.cpp
// Purpose: Hash facility IDs into chains and look them up from there.
// Author: Asanga Rimal

#include "HashTable.h"
#include "StringUtil.h"

// Starts with INITIAL_CAPACITY empty buckets.
HashTable::HashTable() {
    capacity = INITIAL_CAPACITY;
    entryCount = 0;
    collisionCount = 0;
    buckets.resize(capacity, nullptr);
}

// Every node from insert was allocated with new, so free them here.
HashTable::~HashTable() {
    clear();
}

// Deletes every chain. Safe to call more than once.
void HashTable::clear() {
    for (int i = 0; i < capacity; i++) {
        Node* current = buckets[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        buckets[i] = nullptr;
    }
    entryCount = 0;
    collisionCount = 0;
}

// The key here is already uppercase, like F004.
// Start at 0. For each character: hash = (hash * 31 + that character) mod capacity.
// I used 31 because it is a small odd number, so F009 and F010 do not
// all pile into one bucket. The mod is what keeps the index inside the table.
int HashTable::hashFunction(const string& key) const {
    unsigned long hashValue = 0;
    for (int i = 0; i < (int)key.size(); i++) {
        unsigned long letter = (unsigned char)key[i];
        hashValue = (hashValue * 31 + letter) % (unsigned long)capacity;
    }
    return (int)hashValue;
}

// Only walks the chain for this bucket. Stops when the ID matches.
HashTable::Node* HashTable::findNode(const string& key) const {
    int index = hashFunction(key);
    Node* current = buckets[index];
    while (current != nullptr) {
        if (current->facility.getId() == key) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// f001 and F001 are the same key once I trim and uppercase.
// I look for a duplicate before I touch the collision count. If it is
// already there, the first record stays and this insert fails.
// A collision only counts when the new ID goes into a bucket that
// already has a node. I link the new node at the front, so nothing
// already in the chain gets overwritten.
bool HashTable::insert(const Facility& facility) {
    string key = toUpper(trim(facility.getId()));
    if (findNode(key) != nullptr) {
        return false;
    }

    int index = hashFunction(key);
    if (buckets[index] != nullptr) {
        collisionCount++;
    }

    Node* node = new Node(facility);
    node->next = buckets[index];
    buckets[index] = node;
    entryCount++;
    return true;
}

// Same hash as insert. This does not scan the facility vector.
const Facility* HashTable::lookup(const string& id) const {
    string key = toUpper(trim(id));
    Node* node = findNode(key);
    if (node == nullptr) {
        return nullptr;
    }
    return &node->facility;
}

bool HashTable::contains(const string& id) const {
    return lookup(id) != nullptr;
}

int HashTable::getCapacity() const {
    return capacity;
}

int HashTable::getEntryCount() const {
    return entryCount;
}

int HashTable::getCollisionCount() const {
    return collisionCount;
}
