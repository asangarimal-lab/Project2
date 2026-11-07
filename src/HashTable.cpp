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

// Polynomial hash. The key is the uppercase ID.
// hash starts at 0, then for each character c:
//     hash = (hash * 31 + ASCII value of c) mod capacity
// 31 is just a small odd multiplier so IDs that differ by one digit
// do not all collapse the same way. The mod keeps the index in range.
int HashTable::hashFunction(const string& key) const {
    unsigned long hashValue = 0;
    for (int i = 0; i < (int)key.size(); i++) {
        unsigned long letter = (unsigned char)key[i];
        hashValue = (hashValue * 31 + letter) % (unsigned long)capacity;
    }
    return (int)hashValue;
}

// Linear walk of one bucket. Chains are short for this project.
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

// Puts the facility in its bucket. The ID on the facility is already uppercase.
// Duplicate check comes first, so a rejected ID never adds a collision.
// A collision is counted only when the new ID is accepted and the bucket
// already had a node in it.
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

// Same hash and chain walk as insert. Null means not found.
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
