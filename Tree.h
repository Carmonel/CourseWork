#pragma once

#include <vector>

#include <iostream>

class Node{
private:
    bool leaf;
    unsigned char byte = 0;
    std::vector<bool> bits;
    Node* left;
    Node* right;

    friend std::ostream& returnOutStream(std::ostream& os, Node *node, int depth);

public:
    explicit Node(const std::vector<bool> &bits);

    const std::vector<bool>& getBits();

    bool isLeaf() const;

    void setByteAsLeaf(unsigned char byte);
    unsigned char getByte() const;

    void setLeftNode(const Node &node);
    Node* getLeftNode();

    void setRightNode(const Node &node);
    Node* getRightNode();

    ~Node();

    friend std::ostream& operator<<(std::ostream &os, Node &node);
};

inline std::ostream& operator<<(std::ostream &os, Node &node) {
    return returnOutStream(os, &node, 0);
}