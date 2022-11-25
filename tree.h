#pragma once

#include <string>
#include <vector>

class Node{
private:
    int value;
    std::string bits;
    Node* left;
    Node* right;
public:
    Node();
    int returnValue() const;
    explicit Node(const std::string& str);
    void setValue(int value);
    Node* createLeft(const std::string& str);
    Node* createLeft();
    Node* createRight(const std::string& str);
    Node* createRight();
    Node* returnLeftNode();
    Node* returnRightNode();
    std::string returnStr();
    std::string returnStrWithoutNull();
    void setString(const std::string& str);
    ~Node();
};

void print(Node* node, int u);
void returnValues(Node* node, int u, std::vector< std::pair<unsigned char, std::string> > &code);
void makeTree(const size_t* array, size_t size, Node* node);