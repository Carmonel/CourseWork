#include "Archive.h"
#include "utils/Log.h"
#include "Tree.h"
#include "Fano.h"

#include <sstream>

using namespace std;

void addLeafNode(auto &bits, Node &head, char byte){
    Node* thisNode = &head;
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

char readByte(std::ifstream &source, int &chPos, char &ch) {
    int inNumPos = 7; //позиция для байта, куда пишем
    char readedByte = 0;
    while (inNumPos >= 0) {
        while (chPos >= 0 && inNumPos >= 0) {
            readedByte |= (ch & (1 << chPos)); // взяли бит
            inNumPos--;
            chPos--;
        }
        if (chPos < 0) {
            source.read(&ch, 1);
            chPos = 7;
        }
    }
    return readedByte;
}

Node readTree(std::ifstream &source){
    char ch;
    source.read(&ch, 1);
    unsigned char chunkCount = ch; //чанк - байт и последовательность битов, соответствующая ему

    Node head = Node(vector<bool>());

    int chPos = 7; //позиция внутри байта чтения
    size_t readedChunks = 0; //сколько чанков сейчас прочитано

    source.read(&ch, 1); //начали читать

    while (readedChunks < chunkCount) {
        //читаем байт эквивалентный тем, что были в несжатом файле
        char chunkByte = readByte(source, chPos, ch);

        //читаем длину битовой последовательности
        char bitLen = readByte(source, chPos, ch);

        auto bits = vector<bool>();
        bits.push_back(ch & (1 << chPos));
        while (bits.size() < bitLen) {
            if (chPos == 0) {
                source.read(&ch, 1);
                chPos = 7;
            }
            bits.push_back(ch & (1 << chPos));
            chPos--;
        }

        addLeafNode(bits, head, chunkByte);

        readedChunks++;
    }

    return head;
}

void writeDecompressed(std::ofstream& target, Node &treeHead, std::ifstream& sourceData){
    string stream;

    char endBits;
    sourceData.read(&endBits, 1);
    endBits = 8 - endBits;

    char byte;
    Node* thisNode = &treeHead;
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
                thisNode = &treeHead;
            }
            if (stream.length() == 0) break;
            if (stream.at(0) == '0') thisNode = thisNode->getLeftNode();
            if (stream.at(0) == '1') thisNode = thisNode->getRightNode();
            string temp = stream.substr(1);
            stream = temp;
        }
    }
}

Archive::Archive(std::ifstream &input) : input(input) {
}

void Archive::compress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    // Считывание потока данных с файла
    // Заполнение storedUsages, подсчет количества использований
    Fano fanoCodes(input);
    Log::v((stringstream() << fanoCodes).str());

    Log::v("Generating keys...");
    // Генератор keys
    fanoCodes.writeKeys(output);

    Log::v("Generating data stream...");

    // Создание archive
    input.clear();
    input.seekg(0);
    fanoCodes.generateArchived(input, output);
    Log::v("File successfully archived!");
}

void Archive::decompress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    Log::v("Generating tree by stream data...");
    Node treeHead = readTree(input);

    Log::v("Converting stream data to export file...");

    writeDecompressed(output, treeHead, input);

    Log::v("File successfully unpacked!");
}
