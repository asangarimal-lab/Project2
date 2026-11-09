// CampusGraph.h
// Purpose: Undirected graph of campus buildings and walkways, with BFS.
// Author: Sebastian

#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>
using namespace std;

class CampusGraph {
public:
    CampusGraph();

    // Copies the accepted building names in as vertices, in file order.
    void setBuildings(const vector<string>& buildingNames);

    // Reads connections.txt and adds each good line as an edge.
    void loadConnections(const string& path);

    // Prints every building with its neighbors.
    void displayGraph() const;

    // Prints total buildings, total connections, and each degree.
    void displayCountsAndDegrees() const;

    // BFS from the building the user typed. Prints order and distances.
    void bfsReachable(const string& startName) const;

private:
    vector<string> names;          // building names, in insertion order
    vector< vector<int> > adjList; // neighbors of each building, in insertion order
    int edgeCount;                 // each undirected connection counted once

    // Returns the index of a building, or -1 if it is not found.
    int findBuilding(const string& name) const;

    // True if a and b already share an edge.
    bool hasEdge(int a, int b) const;
};

#endif
