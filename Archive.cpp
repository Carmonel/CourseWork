#include "Archive.h"
#include "utils/Log.h"
#include "Tree.h"
#include "Fano.h"

#include <sstream>
#include <bitset>

using namespace std;

void addLeafNode(auto &bits, Node &head, char byte){
    Node* thisNode = &head;
    for (size_t i = 0; i < bits.size(); i++){

        if (bits.at(i) == 0){
            if (thisNode->getLeftNode() == nullptr) {
                thisNode->createLeftNode(vector<bool>(bits.begin(), bits.begin() + i));
            }
            thisNode = thisNode->getLeftNode();
        } else {
            if (thisNode->getRightNode() == nullptr) {
                thisNode->createRightNode(vector<bool>(bits.begin(), bits.begin() + i));
            }
            thisNode = thisNode->getRightNode();

        }
    }
    thisNode->setByteAsLeaf(byte);
}

char readByte(std::ifstream &source, int &chPos, char &ch) {
    //Log::d("==for byte " + (stringstream() << bitset<8>(ch)).str());
    int inNumPos = 7; //позиция для байта, куда пишем
    char readedByte = 0;
    while (inNumPos >= 0) {
        //Log::d("readed byte " + (stringstream() << bitset<8>(readedByte)).str());
        while (chPos >= 0 && inNumPos >= 0) {
            //Log::d("chPos " + to_string(chPos));
            //Log::d("inNumPos " + to_string(inNumPos));
            unsigned char bit = (ch & (1 << chPos)); // взяли бит
            int seek = inNumPos - chPos; //смотрим куда двигать бит, чтобы занести в tempByte
            //Log::d("seek " + to_string(seek));
            if (seek < 0) {
                readedByte |= bit >> -seek;
            } else {
                readedByte |= bit << seek;
            }
            inNumPos--;
            chPos--;
        }
        if (chPos < 0) {
            source.read(&ch, 1);
            //Log::d("read new byte " + (stringstream() << bitset<8>(ch)).str());
            chPos = 7;
        }
    }
    return readedByte;
}

Node readTree(std::ifstream &source){
    char ch;
    source.read(&ch, 1);
    //чанк - байт и последовательность битов, соответствующая ему
    size_t chunkCount = ch;
    if (ch == 0){
        chunkCount = 256;//костыль для количества чанков 256 (max char = 255)
    }
    //Log::d("chunkCount " + to_string(chunkCount));

    Node head = Node(vector<bool>());

    int chPos = 7; //позиция внутри байта чтения
    size_t readedChunks = 0; //сколько чанков сейчас прочитано

    source.read(&ch, 1); //начали читать

    while (readedChunks < chunkCount) {
        //Log::d("readed chunks " + to_string(readedChunks));
        //читаем байт эквивалентный тем, что были в несжатом файле
        unsigned char chunkByte = readByte(source, chPos, ch);
        //Log::d("---------chunkByte " + to_string(chunkByte) + "  " + (stringstream() << bitset<8>(chunkByte)).str());

        //читаем длину битовой последовательности
        unsigned char bitsLen = readByte(source, chPos, ch);
        //Log::d("----bitsLen " + to_string(bitsLen) + "  " + (stringstream() << bitset<8>(bitsLen)).str());

        auto bits = vector<bool>();

        while (bits.size() < bitsLen) {
            if (chPos == 0) {
                bits.push_back(ch & (1 << chPos));
                source.read(&ch, 1);
                chPos = 7;
            } else {
                bits.push_back(ch & (1 << chPos));
                chPos--;
            }
        }

        stringstream ss;
        for (bool b : bits) {
            ss << b;
        }

        //Log::d("readed bits " + ss.str());

        addLeafNode(bits, head, chunkByte);

        readedChunks++;
    }

    return head;
}

string vbtos(const vector<bool> &bits){
    stringstream ss;
    for (bool b : bits){
        ss << b;
    }
    return ss.str();
}

void writeDecompressed(std::ifstream& source, std::ofstream& target, Node *treeHead){
    Node* curNode = treeHead;

    int chPos = 7, chEnd = -1;
    char ch1, ch2, ch3;
    char toByte;
    bool workFlag = true;
    source.read(&ch2, 1);
    source.read(&ch3, 1); //ch3 костыль, иначе не ловится eof, потому нужно помнить 3 байта

    //Будем тупо читать биты пока не дойдем до листа, если найден лист, пишем байт в target
    while (workFlag) {
        chPos = 7;
        ch1 = ch2;
        ch2 = ch3;
        source.read(&ch3, 1);//чтобы получить длину хвоста если конец файла

        if (source.eof()) {
            workFlag = false;
            chEnd = ch3;
        }
        while (chPos > chEnd) {
            if ((unsigned char)(ch1 & (1 << chPos)) > 0) {
                curNode = curNode->getRightNode();
            } else {
                curNode = curNode->getLeftNode();
            }
            chPos--;
            if (curNode->isLeaf()) {
                toByte = curNode->getByte();
                target.write(&toByte, 1);
                curNode = treeHead;
            }
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
    //Log::v((stringstream() << fanoCodes).str());

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

#include <stack>

std::vector< std::pair<unsigned char, std::vector<bool>>>
mapToArray(Node *head){
    auto mapped = std::vector< std::pair<unsigned char, std::vector<bool>>>();
    stack<Node*> stackNodes = stack<Node*>();
    stackNodes.push(head);

    while (!stackNodes.empty()){
        Node* cur = stackNodes.top();
        if (cur->isLeaf()){
            mapped.emplace_back(cur->getByte(), cur->getBits());
            stackNodes.pop();
        } else {
            stackNodes.pop();
            if (cur->getRightNode() != nullptr) stackNodes.push(cur->getRightNode());
            if (cur->getLeftNode() != nullptr) stackNodes.push(cur->getLeftNode());
        }
    }

    return mapped;
}

void Archive::decompress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    Log::v("Generating tree by stream data...");
    Node treeHead = readTree(input);
    auto d = mapToArray(&treeHead);
    for (auto &p : d) {
        string dd("");
        for (bool b : p.second) {
            dd += to_string(b);
        }
       // Log::d(to_string(p.first) + "|" + dd);
    }

    Log::v("Converting stream data to export file...");

    writeDecompressed(input, output, &treeHead);

    Log::v("File successfully unpacked!");
}