//
// Created by NHTHS on 25.11.22.
//

#include "Archive.h"
#include "Log.h"
#include "Exception.h"
#include "tree.h"
#include "Fano.h"

#include <sstream>
#include <fstream>

using namespace std;

void readInt(std::ifstream &source, string& binary, int& valueExport){
    // Считывается значение
    char value;
    source.read(&value, 1);
    valueExport = value;
    // Считывание количества байт
    char bitsCount;
    source.read(&bitsCount, 1);

    while (bitsCount > 0){
        // Считывание нулей
        char nulls;
        source.read(&nulls, 1);
        // Считывание части строки
        char byte;
        source.read(&byte, 1);

        string bits;
        unsigned char bit = byte;
        while (bit){
            bits.push_back((bit & 1) + '0');
            bit >>= 1;
        }
        std::reverse(bits.begin(), bits.end());
        while (nulls > 0){
            bits.insert(0, "0");
            nulls--;
        }
        binary += bits;

        bitsCount--;
    }
}

Node* readTree(std::ifstream &source){
    Node* head = new Node;
    char count;
    source.read(&count, 1);

    while (count > 0) {
        int value;
        string bits;

        readInt(source, bits, value);

        Node* thisNode = head;
        string str;
        for (int i = 0; i < bits.length(); i++){
            if (bits.at(i) == '0'){
                if (thisNode->returnLeftNode() == nullptr) thisNode = thisNode->createLeft(str);
                else thisNode = thisNode->returnLeftNode();
                str = thisNode->returnStr();
                continue;
            }
            if (bits.at(i) == '1'){
                if (thisNode->returnRightNode() == nullptr) thisNode = thisNode->createRight(str);
                else thisNode = thisNode->returnRightNode();
                str = thisNode->returnStr();
            }
        }
        thisNode->setValue(value);
        count--;
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
            if ((thisNode->returnLeftNode() == nullptr) && (thisNode->returnRightNode() == nullptr)){
                char forHex = (char)thisNode->returnValue();
                target.write(&forHex, 1);
                thisNode = treeHead;
            }
            if (stream.length() == 0) break;
            if (stream.at(0) == '0') thisNode = thisNode->returnLeftNode();
            if (stream.at(0) == '1') thisNode = thisNode->returnRightNode();
            string temp = stream.substr(1);
            stream = temp;
        }
    }
}

// Переопределение пути в вид \\root\\DECOMPILED[file]
string resultFileName(string path){
    path.erase(path.length()-8);
    int it = path.length();
    while (path[it] != '/') it--;
    it++;
    path.insert(it, "DECOMPILED");

    return path;
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


Archive::Archive(const std::string &path) {
    this->path = path;
}

void Archive::compress(const std::string &outPath) {
    std::ios_base::sync_with_stdio(false);

    Log::v("Working with '" + path + "'");

    // Считывание потока данных с файла
    // Заполнение storedUsages, подсчет количества использований
    Fano main(path);

    Log::v((stringstream() << main).str());

    Log::v("Generating keys...");

    if (!main.isEfficiency()){
        Log::v("Attention: this file compressed inefficient.");
        main.generateInefficient(path);

        Log::v(path + ".archive  -  created.");
        Log::v("File successfully archived!");
        return;
    }

    // Генератор keys
    main.generateKeyFile(path + ".archive");

    Log::v("Generating data stream...");

    // Создание archive
    main.generateArchived(path);

    Log::v(path + ".archive  -  created.");
    Log::v("File successfully archived!");
}

void Archive::decompress(const std::string &outPath) {
    std::ios_base::sync_with_stdio(false);

    Log::v("Working with '" + path + "'");

    std::ifstream streamFile(path, std::ios::binary | std::ios::in);
    if (streamFile.fail()){
        throw FileOpenError("Opening file failed");
    }

    string newPath(resultFileName(path));

    std::ofstream decompiledFile(newPath, std::ios::binary | std::ios::out);

    if (!decompiledFile.fail()) decompiledFile.clear();
    if (decompiledFile.fail()){
        throw FileOpenError("Opening file failed");
    }

    if (isInefficient(streamFile)){
        decompressInefficient(streamFile, decompiledFile);
        Log::v("" + newPath + " - created!");
        Log::v("File successfully unpacked!");
        return;
    }

    Log::v("Generating tree by stream data...");
    Node* treeHead = readTree(streamFile);

    Log::v("Converting stream data to export file...");
    Log::v("" + newPath + " - created!");

    writeDecompressed(decompiledFile, treeHead, streamFile);
    decompiledFile.close();

    Log::v("File successfully unpacked!");
}
