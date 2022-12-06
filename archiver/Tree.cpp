#include "Tree.h"
#include "../utils/Log.h"

using namespace std;

Node::Node(const vector<bool> &bits) {
    leaf = false;
    byte = 0;
    this->bits = vector<bool>(bits);
    left = nullptr;
    right = nullptr;
}

Node::~Node() {
    //TODO
}

void Node::createLeftNode(const vector<bool> &bits) {
    left = new Node(bits);
}

Node *Node::getLeftNode() {
    return left;
}

void Node::createRightNode(const vector<bool> &bits) {
    right = new Node(bits);
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
    for (bool b : vec) {
        os << b;
    }
    return os;
}

std::ostream &returnOutStream(ostream &os, Node *node, int depth) {
    if (node->isLeaf()) {
        os << (unsigned int)(node->byte) << "|(" << node->bits.size() << ") " << node->bits << endl;
    }

    Node* child = node->left;
    if (child != nullptr) returnOutStream(os, child, depth+1);

    child = node -> right;
    if (child != nullptr) returnOutStream(os, child, depth+1);
    return os;
}