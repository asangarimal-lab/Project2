// HashTable.h
// Purpose: Custom hash table keyed by facility ID, with separate chaining.
// Author: Asanga Rimal
//
// This is our own table. It does not use the library hash or unordered_map.
// Capacity is the number of buckets. Chains can grow, so there is no max
// number of facilities.

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "Facility.h"
using namespace std;

class HashTable {
private:
    // One node in a bucket's linked list.
    struct Node {
        Facility facility;
        Node* next;

        Node(const Facility& item) {
            facility = item;
            next = nullptr;
        }
    };

    // Small prime. With the hash below, some sample IDs share a bucket,
    // which is what we want when we print the collision count.
    static const int INITIAL_CAPACITY = 11;

    vector<Node*> buckets;
    int capacity;
    int entryCount;
    int collisionCount;

    // Turns an uppercase ID into a bucket index.
    int hashFunction(const string& key) const;

    // Walks one chain. Returns the node with this ID, or null.
    Node* findNode(const string& key) const;

    // Frees every node. Used by the destructor.
    void clear();

public:
    HashTable();
    ~HashTable();

    // The chains belong to this table, so a copy would delete them twice.
    HashTable(const HashTable& other) = delete;
    HashTable& operator=(const HashTable& other) = delete;

    // Inserts a facility. Returns false if that ID is already stored.
    // A false return does not change the collision count.
    bool insert(const Facility& facility);

    // Hash lookup. Returns null when the ID is not in the table.
    // The pointer stays valid until the table is destroyed.
    const Facility* lookup(const string& id) const;

    // True when the uppercase ID is already an entry.
    bool contains(const string& id) const;

    int getCapacity() const;
    int getEntryCount() const;
    int getCollisionCount() const;
};

#endif
