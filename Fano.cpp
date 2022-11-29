#include "Fano.h"
#include "utils/Log.h"
#include "utils/Exception.h"

#include <map>
#include <sstream>
#include <vector>
#include <stack>
#include <stdlib.h>
#include <bitset>

using namespace std;

// https://iq.opengenus.org/sorting-vector-in-cpp/
bool operator<(pair<unsigned char, size_t> &a, pair<unsigned char, size_t> &b){
    return a.second > b.second;
}

// [Создание дерева]
// Функция с рекурсией, что разделяет массив на две части так, что разность сумм двух частей была минимальна.
// Рекурсия состоит в том, что необходимо разделить массив неизвестного размера, поэтому функция разделяет массив и подмассивы
// до того момента, как размер новой части массива не станет равна единице (всего один элемент в массиве).
//
// Для перехода на следующий часть в конце алгоритма вызывается эта же функция, но для левого подмассива вызывается прошлый
// указатель на начало массива-родителя, а для правого вызывается указатель на место разрыва массива-родителя.
// Для каждого случая так же вызывается размер массива.
void makeTree(const pair<unsigned char, size_t>* array, size_t arraySize, Node* node){
    // [Конец рекурсии]
    // Создание листьев, если у части массива остался 1 элемент
    if (arraySize == 1){
        node->setByteAsLeaf(array[0].first); // Ввод получившегося символа в лист дерева
        return;
    }

    // [Основной алгоритм]
    // Нахождение середины массива так, что, разделив его, сумма элементов из частей массива имеет минимальную разницу.
    // 1. Создание дополнительного массива для запоминания разности сумм при различных местах разрыва.
    // 2.1 Цикличный проход по всем элементам от 0 до size-1.
    // 2.2 Если предыдущая разность была меньше чем нынешняя, то возврат места разрыва на прошлый элемент.
    //
    // differenceHolder[] - массив для сохранения результатов подсчета разностей на каждом шаге.
    // index - значение, показывающее где необходим разрыв.
    auto* differenceHolder = new size_t[arraySize];
    size_t index = 0;

    // Шаг 2.1
    for (index = 0; index < arraySize; index++){

        long long leftSum = 0; // Подсчет левой части
        long long rightSum = 0; // Подсчет правой части

        // Заполнение left и right циклом, что проходит от 0 до size-1
        // Если мы на элементе левее нынешнего index, то увеличивается leftSum
        // Если мы на элементе правее нынешнего index, то увеличивается rightSum
        for (int j = 0; j < arraySize; j++){
            if (j < index) leftSum = leftSum + array[j].second;
            else rightSum =  rightSum + array[j].second;
        }
        differenceHolder[index] = abs(leftSum - rightSum); // Запоминание разницы

        // Шаг 2.2
        if ((index != 0) && (differenceHolder[index] > differenceHolder[index - 1])) {
            index--;
            break;
        }
    }

    // Если середина в конце, то справа будет массив без элементов, возврат на элемент назад
    if (index == arraySize) index--;

    // Удаление differenceHolder за ненадобностью далее
    delete[] differenceHolder;

    // [Рекурсия]
    // Для построения дерева с весами '0' или '1', при прохождении влево добавляется '0', при прохождении направо '1'
    // Для левого массива, копирование битов с прошлого нода.
    // Для правого массива, копирование битов с прошлого нода.
    // В качестве аргументов в createLeft(...) входит returnStr() для записывания '0' или '1' к прошлому значению битов.
    auto nextBits = vector<bool>(node->getBits());

    nextBits.push_back(0);
    node->setLeftNode(Node(nextBits));

    nextBits.at(nextBits.size() - 1) = 1;
    node->setRightNode(Node(nextBits));

    makeTree(array, index, node->getLeftNode());
    makeTree(array + index, arraySize - index, node->getRightNode());
}

vector<pair<unsigned char, size_t>> mapToSortedVector(map<unsigned char, size_t> &map){
    auto tempArr = vector<pair<unsigned char, size_t>>();

    for (unsigned char i = 0; i < UCHAR_MAX; i++) {
        tempArr.emplace_back(*map.find(i));
    }
    sort(tempArr.begin(), tempArr.end());
    return tempArr;
}

std::vector< std::pair<unsigned char, std::vector<bool>>>
        mapToBytesArray(Node *head){
    auto mapped = std::vector< std::pair<unsigned char, std::vector<bool>>>();
    stack<Node*> stackNodes = stack<Node*>();
    stackNodes.push(head);

    while (!stackNodes.empty()){
        auto cur = stackNodes.top();
        if (cur->isLeaf()){
            mapped.emplace_back(cur->getByte(), cur->getBits());
            stackNodes.pop();
        } else {
            stackNodes.pop();
            stackNodes.push(cur->getRightNode());
            stackNodes.push(cur->getLeftNode());
        }
    }

    return mapped;
}

void removeRedudantZeros(std::vector<std::pair<unsigned char, std::vector<bool>>> &vec){
    for (auto &p : vec) {
        auto &bits = p.second;
        size_t i = 0;
        for (i = 0; i < bits.size() - 1; ++i) {
            if (bits[i] == 1) {
                break;
            }
        }
        if (i > 0) {
            bits.erase(bits.begin(), bits.begin() + i);
        }
    }
}

Fano::Fano(std::istream &input) {
    map<unsigned char, size_t> bytesFrequency = map<unsigned char, size_t>();

    // Ввод символов
    unsigned char ch;
    while (input.read(reinterpret_cast<char *>(&ch), 1)){
        bytesFrequency[ch]++;
    }

    Log::d("frequency");
    for (auto &p : bytesFrequency) {
        Log::d(to_string(p.first) + "|" + to_string(p.second));
    }

    auto tempArr = mapToSortedVector(bytesFrequency);

    Node head = Node(vector<bool>());

    makeTree(tempArr.cbegin().base(), tempArr.size(), &head);
    tempArr.clear();

    storedCode = mapToBytesArray(&head);
    removeRedudantZeros(storedCode);
}

Fano::~Fano() {
    storedCode.clear();
}

void writeByte(std::ostream &out, char byte, char &usedByte, int &inBytePos){
    int inNumPos = 7;
    while (inNumPos >= 0) {
        while (inBytePos >= 0 && inNumPos >= 0) {
            char bit = (byte & (1 << inNumPos)); // взяли бит
            int seek = inBytePos - inNumPos; //смотрим куда двигать бит, чтобы занести в tempByte
            if (seek < 0) {
                usedByte |= bit >> -seek;
            } else {
                usedByte |= bit << seek;
            }
            inBytePos--;
            inNumPos--;
        }
        if (inBytePos < 0) {
            out.write(&usedByte, 1);
            usedByte = 0;
            inBytePos = 7;
        }
    }
}

void Fano::writeKeys(std::ostream &out) {
    char size = storedCode.size();
    out.write(&size, 1);

    Log::d("Write key loop");

    int inBytePos = 7; //позиция внутри tempByte (будем идти со старшего бита к младшему)
    char tempByte = 0; //байт для записи
    for (auto &p : storedCode){
        //пишем байт чанка
        writeByte(out, p.first, tempByte, inBytePos);

        //пишем длинну битов чанка как 1 байтовое число
        writeByte(out, p.second.size(), tempByte, inBytePos);

        //пишем биты
        for (bool b : p.second) {
            tempByte |= (b << inBytePos);
            if (inBytePos == 0) {
                out.write(&tempByte, 1);
                tempByte = 0;
                inBytePos = 7;
            }
            inBytePos--;
        }
    }
    if (tempByte != 0) { //дозаписываем остаток, который мог не записаться в цикле
        out.write(&tempByte, 1);
    }
}

void Fano::generateArchived(std::istream &input, std::ostream &out) {
    Log::v("Scanning and writing... ");

    auto codes = map(storedCode.begin(), storedCode.end());

    char ch = 0; //сначала читаем им, потом сразу же в него и пишем

    int inBytePos = 7; //позиция внутри tempByte (будем идти со старшего бита к младшему)
    char tempByte = 0; //байт для записи

    while(input.read(&ch, 1)){
        auto &bits = codes[ch];
        //пишем биты
        for (bool b : bits) {
            tempByte |= (b << inBytePos);
            if (inBytePos == 0) {
                out.write(&tempByte, 1);
                tempByte = 0;
                inBytePos = 7;
            }
            inBytePos--;
        }
    }
}