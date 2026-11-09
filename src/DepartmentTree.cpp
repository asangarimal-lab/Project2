#include <fstream>
#include <iostream>
#include "DepartmentTree.h"
#include "StringUtil.h"
using namespace std;

DepartmentTree::DepartmentTree() : root(nullptr) {}

DepartmentTree::~DepartmentTree() {
    deleteTree(root);
}

bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

DepartmentTree::Node* DepartmentTree::findNode(
    Node* node, const string& name) const {

    if (node == nullptr) {
        return nullptr;
    }

    if (equalsIgnoreCase(node->name, name)) {
        return node;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        Node* found = findNode(node->children[i], name);

        if (found != nullptr) {
            return found;
        }
    }

    return nullptr;
}

void DepartmentTree::deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        deleteTree(node->children[i]);
    }

    delete node;
}

bool DepartmentTree::addDepartment(
    const string& name, const string& parentName, string& error) {

    error.clear();

    string departmentName = trim(name);
    string parent = trim(parentName);

    if (departmentName.empty()) {
        error = "department name cannot be blank";
        return false;
    }

    if (parent.empty()) {
        error = "parent name cannot be blank";
        return false;
    }

    if (findNode(root, departmentName) != nullptr) {
        error = "duplicate department name " + departmentName;
        return false;
    }

    if (root == nullptr) {
        if (!equalsIgnoreCase(parent, "NONE")) {
            error = "first department must have parent NONE";
            return false;
        }

        root = new Node(departmentName, nullptr);
        return true;
    }

    if (equalsIgnoreCase(parent, "NONE")) {
        error = "only one root department is allowed";
        return false;
    }

    Node* parentNode = findNode(root, parent);

    if (parentNode == nullptr) {
        error = "unknown parent " + parent;
        return false;
    }

    Node* child = new Node(departmentName, parentNode);

    try {
        parentNode->children.push_back(child);
    } catch (...) {
        delete child;
        throw;
    }

    return true;
}

void DepartmentTree::loadFromFile(const string& path) {
    const string label = "departments.txt";
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

        string text = trim(line);

        if (text.empty() || text[0] == '#') {
            continue;
        }

        vector<string> fields = splitFields(line);
        string error;

        if (fields.size() != 2) {
            error = "expected 2 fields (DepartmentName|ParentName)";
        } else if (addDepartment(fields[0], fields[1], error)) {
            accepted++;
            continue;
        }

        cout << "Warning: " << label << " line " << lineNumber
             << ": " << error << endl;

        rejected++;
    }

    cout << label << ": " << accepted << " accepted, "
         << rejected << " rejected" << endl;
}

void DepartmentTree::searchDepartment(const string& name) const {
    string departmentName = trim(name);

    if (departmentName.empty()) {
        cout << "Error: department name cannot be blank." << endl;
        return;
    }

    if (isEmpty()) {
        cout << "Department tree is empty." << endl;
        return;
    }

    Node* node = findNode(root, departmentName);

    if (node == nullptr) {
        cout << "Department not found: " << departmentName << endl;
        return;
    }

    int depth = 0;

    for (Node* parent = node->parent;
         parent != nullptr; parent = parent->parent) {
        depth++;
    }

    cout << "Name: " << node->name << endl;

    cout << "Parent: "
         << (node->parent == nullptr ? "None" : node->parent->name)
         << endl;

    cout << "Child count: " << node->children.size() << endl;
    cout << "Depth: " << depth << endl;
}

void DepartmentTree::printHierarchy(Node* node, int depth) const {
    cout << string(depth * 4, ' ') << node->name << endl;

    for (size_t i = 0; i < node->children.size(); i++) {
        printHierarchy(node->children[i], depth + 1);
    }
}

void DepartmentTree::displayHierarchy() const {
    if (isEmpty()) {
        cout << "Department tree is empty." << endl;
        return;
    }

    printHierarchy(root, 0);
}

void DepartmentTree::printPreorder(Node* node, bool& first) const {
    if (!first) {
        cout << " -> ";
    }

    cout << node->name;
    first = false;

    for (size_t i = 0; i < node->children.size(); i++) {
        printPreorder(node->children[i], first);
    }
}

void DepartmentTree::displayPreorder() const {
    if (isEmpty()) {
        cout << "Department tree is empty." << endl;
        return;
    }

    bool first = true;
    printPreorder(root, first);
    cout << endl;
}