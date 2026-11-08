#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>
using namespace std;

class DepartmentTree {
private:
    struct Node {
        string name;
        Node* parent;
        vector<Node*> children;

        Node(const string& departmentName, Node* parentNode)
            : name(departmentName), parent(parentNode) {}
    };

    Node* root;

    Node* findNode(Node* node, const string& name) const;
    void deleteTree(Node* node);
    void printHierarchy(Node* node, int depth) const;
    void printPreorder(Node* node, bool& first) const;

    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const;

    bool addDepartment(const string& name, const string& parentName,
                       string& error);

    void loadFromFile(const string& path);
    void searchDepartment(const string& name) const;
    void displayHierarchy() const;
    void displayPreorder() const;
};

#endif