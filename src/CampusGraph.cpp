// CampusGraph.cpp
// Purpose: Adjacency list graph for the campus, plus connection loading and BFS.
// Author: Sebastian

#include <fstream>
#include <iostream>
#include <queue>
#include "CampusGraph.h"
#include "StringUtil.h"
using namespace std;

// Starts with no buildings and no connections.
CampusGraph::CampusGraph() {
    edgeCount = 0;
}

// Each building becomes a vertex with an empty neighbor list.
void CampusGraph::setBuildings(const vector<string>& buildingNames) {
    names = buildingNames;
    adjList.clear();
    adjList.resize(names.size());
    edgeCount = 0;
}

// Trims the typed name and ignores letter case when matching.
int CampusGraph::findBuilding(const string& name) const {
    string cleaned = trim(name);
    for (int i = 0; i < (int)names.size(); i++) {
        if (equalsIgnoreCase(names[i], cleaned)) {
            return i;
        }
    }
    return -1;
}

// Looks through a's neighbors for b.
bool CampusGraph::hasEdge(int a, int b) const {
    for (int i = 0; i < (int)adjList[a].size(); i++) {
        if (adjList[a][i] == b) {
            return true;
        }
    }
    return false;
}

// Reads connections.txt. Bad lines get a warning and are skipped.
void CampusGraph::loadConnections(const string& path) {
    string label = "connections.txt";
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
        // only a # at the start makes it a comment
        if (trimmed[0] == '#') {
            continue;
        }

        vector<string> fields = splitFields(line);
        if (fields.size() != 2) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": expected 2 fields (BuildingA|BuildingB)" << endl;
            rejected++;
            continue;
        }
        if (fields[0].empty() || fields[1].empty()) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": empty building name" << endl;
            rejected++;
            continue;
        }

        int a = findBuilding(fields[0]);
        int b = findBuilding(fields[1]);
        if (a == -1) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": unknown building " << fields[0] << endl;
            rejected++;
            continue;
        }
        if (b == -1) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": unknown building " << fields[1] << endl;
            rejected++;
            continue;
        }
        if (a == b) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": building connected to itself (" << names[a] << ")" << endl;
            rejected++;
            continue;
        }
        // hasEdge also catches A|B after B|A
        if (hasEdge(a, b)) {
            cout << "Warning: " << label << " line " << lineNumber
                 << ": duplicate connection " << names[a] << " - " << names[b] << endl;
            rejected++;
            continue;
        }

        // add to both ends but count the edge only once
        adjList[a].push_back(b);
        adjList[b].push_back(a);
        edgeCount++;
        accepted++;
    }

    cout << label << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}

// Shows each building and its neighbors in the order they were added.
void CampusGraph::displayGraph() const {
    if (names.empty()) {
        cout << "No buildings are loaded." << endl;
        return;
    }

    cout << "Campus graph:" << endl;
    for (int i = 0; i < (int)names.size(); i++) {
        cout << names[i] << ": ";
        if (adjList[i].empty()) {
            cout << "(no connections)";
        } else {
            for (int j = 0; j < (int)adjList[i].size(); j++) {
                if (j > 0) {
                    cout << ", ";
                }
                cout << names[adjList[i][j]];
            }
        }
        cout << endl;
    }
}

// Degree is just how many neighbors a building has.
void CampusGraph::displayCountsAndDegrees() const {
    if (names.empty()) {
        cout << "No buildings are loaded." << endl;
        cout << "Total buildings: 0" << endl;
        cout << "Total connections: 0" << endl;
        return;
    }

    cout << "Total buildings: " << names.size() << endl;
    cout << "Total connections: " << edgeCount << endl;
    cout << "Degree of each building:" << endl;
    for (int i = 0; i < (int)names.size(); i++) {
        cout << "  " << names[i] << ": " << adjList[i].size() << endl;
    }
}

// Breadth-first search. I mark a building visited when I put it in the queue.
void CampusGraph::bfsReachable(const string& startName) const {
    if (names.empty()) {
        cout << "No buildings are loaded." << endl;
        return;
    }

    int start = findBuilding(startName);
    if (start == -1) {
        cout << "Error: unknown building " << trim(startName) << endl;
        return;
    }

    vector<bool> visited(names.size(), false);
    vector<int> distance(names.size(), 0);
    queue<int> toVisit;

    visited[start] = true;
    toVisit.push(start);

    int reachable = 0;
    cout << "Reachable buildings from " << names[start] << ":" << endl;
    while (!toVisit.empty()) {
        int current = toVisit.front();
        toVisit.pop();
        reachable++;
        cout << names[current] << " (distance " << distance[current] << ")" << endl;

        for (int i = 0; i < (int)adjList[current].size(); i++) {
            int next = adjList[current][i];
            if (!visited[next]) {
                visited[next] = true;
                distance[next] = distance[current] + 1;
                toVisit.push(next);
            }
        }
    }

    cout << "Reachable: " << reachable << " of " << names.size() << " buildings" << endl;
}
