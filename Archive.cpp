//
// Created by NHTHS on 25.11.22.
//

#include "Archive.h"
#include "utils/Log.h"
#include "Tree.h"
#include "Fano.h"

#include <sstream>

using namespace std;

vector<bool> readBits(std::ifstream &source){
    auto bits = vector<bool>();

    // Считывание количества байт
    char bytesCount;
    source.read(&bytesCount, 1);

    char temp;
    source.read(&temp, 1);

    char pos = 7;
    while (temp >> pos == 0) {
        pos--;
    }

    for (unsigned char i = 0; i < (unsigned char)(bytesCount); i++) {
        char temp;
        char mask = 1 << pos;
        source.read(&temp, 1);

        for (; pos >= 0; pos--) {
            bits.push_back(temp & mask);
            mask >>= 1;
        }
        pos = 7;
    }

    return bits;
}

char readByte(std::ifstream &source){
    char value;
    source.read(&value, 1);
    return value;
}

Node* readTree(std::ifstream &source){
    Node* head = new Node(vector<bool>());
    char count;
    source.read(&count, 1);

    for (; count > 0; count--) {
        char byte = readByte(source);
        auto bits = readBits(source);

        Node* thisNode = head;

        for (int i = 0; i < bits.size(); i++){
            Node* tempNode = nullptr;

            if (bits[i] == 0){
                tempNode = thisNode->getLeftNode();
                if (tempNode == nullptr) {
                    thisNode->setLeftNode(Node(vector<bool>(bits.begin(), bits.begin() + i)));
                    tempNode = thisNode->getLeftNode();
                }
                thisNode = tempNode;

            } else {
                tempNode = thisNode->getRightNode();
                if (tempNode == nullptr) {
                    thisNode->setRightNode(Node(vector<bool>(bits.begin(), bits.begin() + i)));
                    tempNode = thisNode->getRightNode();
                }
                thisNode = tempNode;

            }
        }
        thisNode->setByteAsLeaf(byte);
    }
    return head;
}

void writeDecompressed(std::ofstream& target, Node* treeHead, std::ifstream& sourceData){
    string stream;

    char endBits;
    sourceData.read(&endBits, 1);
    endBits = 8 - endBits;

    char byte;
    Node* thisNode = treeHead;
    while (sourceData.read(&byte, 1)){
        int a = 0;
        string charToBin;
        unsigned char ch = byte;
        while (ch){
            a++;
            if (ch & 1) charToBin += '1';
            else charToBin += '0';
            ch >>= 1;
        }
        std::reverse(charToBin.begin(), charToBin.end());
        while (a < 8){
            charToBin.insert(0, "0");
            a++;
        }
        stream += charToBin;

        if (sourceData.peek() == EOF){
            while (endBits > 0){
                stream.pop_back();
                endBits--;
            }
        }
        while (true){
            if (thisNode->isLeaf()){
                char forHex = thisNode->getByte();
                target.write(&forHex, 1);
                thisNode = treeHead;
            }
            if (stream.length() == 0) break;
            if (stream.at(0) == '0') thisNode = thisNode->getLeftNode();
            if (stream.at(0) == '1') thisNode = thisNode->getRightNode();
            string temp = stream.substr(1);
            stream = temp;
        }
    }
}

void decompressInefficient(std::ifstream& streamFile, std::ofstream& decompiledFile){
    Log::v("Copying data from stream file to decompiled file...");
    Log::v("Converting stream data to export file...");
    char ch;
    while (streamFile.read(&ch, 1)) decompiledFile.write(&ch, 1);
}

bool isInefficient(std::ifstream& file){
    char check;
    file.read(&check, 1);
    if (check == 0){
        Log::v("Attention: this file was compressed inefficient.");
        return true;
    }
    return false;
}


Archive::Archive(std::ifstream &input) : input(input) {
}

void Archive::compress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    // Считывание потока данных с файла
    // Заполнение storedUsages, подсчет количества использований
    Fano main(input);
    Log::v((stringstream() << main).str());

    if (!main.isEfficiency()){
        Log::i("Attention: this file compressed inefficient.");
        main.generateInefficient(input, output);
        Log::v("File successfully archived!");
        return;
    }

    Log::v("Generating keys...");
    // Генератор keys
    main.writeKeys(output);

    Log::v("Generating data stream...");

    // Создание archive
    input.clear();
    input.seekg(0);
    main.generateArchived(input, output);
    Log::v("File successfully archived!");
}

void Archive::decompress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    if (isInefficient(input)){
        decompressInefficient(input, output);
        Log::v("File successfully unpacked!");
        return;
    }

    Log::v("Generating tree by stream data...");
    Node* treeHead = readTree(input);

    Log::v("Converting stream data to export file...");

    writeDecompressed(output, treeHead, input);

    Log::v("File successfully unpacked!");
}
