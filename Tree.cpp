#include "Tree.h"

using namespace std;

Node::Node(const vector<bool> &bits) {
    leaf = false;
    byte = 0;
    this->bits = bits;
    left = nullptr;
    right = nullptr;
}

Node::~Node() {
    delete left;
    delete right;
}

void Node::setLeftNode(const Node &node) {
    left = new Node(node);
}

Node *Node::getLeftNode() {
    return left;
}

void Node::setRightNode(const Node &node) {
    right = new Node(node);
}

Node *Node::getRightNode() {
    return right;
}

const std::vector<bool> &Node::getBits() {
    return bits;
}

bool Node::isLeaf() const {
    return leaf;
}

void Node::setByteAsLeaf(unsigned char byte) {
    this->byte = byte;
    leaf = true;
}

unsigned char Node::getByte() const {
    return byte;
}

ostream& operator<<(ostream &os, vector<bool>& vec){
    for (_Bit_reference b : vec) {
        os << b;
    }
    return os;
}

std::ostream &returnOutStream(ostream &os, Node *node, int depth) {
    for (int i = 0; i < depth; i++){
        os << "      ";
    }

    os << node->byte << "|" << node->bits << endl;

    Node* child = node->left;
    if (child != nullptr) returnOutStream(os, child, depth+1);

    child = node -> right;
    if (child != nullptr) returnOutStream(os, child, depth+1);
    return os;
}