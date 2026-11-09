// HashTable.h
// Purpose: My hash table for facility IDs, using separate chaining.
// Author: Asanga Rimal
//
// I wrote the hash myself. This is not unordered_map and it does not call
// the library hash. 11 is how many buckets we start with. A chain can get
// longer, so that number is not a cap on facilities.

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

    // 11 is small on purpose. F009 and F010 share a bucket, so the
    // load summary has a real collision to show.
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

    // False means that ID was already there. The old record stays,
    // and a rejected duplicate does not add to the collision count.
    bool insert(const Facility& facility);

    // Hashes the ID and walks that one chain. Null means it is not there.
    const Facility* lookup(const string& id) const;

    // True when the uppercase ID is already an entry.
    bool contains(const string& id) const;

    int getCapacity() const;
    int getEntryCount() const;
    int getCollisionCount() const;
};

#endif
