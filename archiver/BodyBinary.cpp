#include "BodyBinary.h"
#include "../utils/Log.h"
#include <map>

using namespace std;

void BodyBinary::writeCompressedToFile(std::ifstream &source, std::ostream &dest,
                                       std::vector<std::pair<unsigned char, std::vector<bool>>>  &codes) {
    Log::v("Scanning and writing... ");

    auto codesMap = map(codes.begin(), codes.end());

    char ch = 0; //сначала читаем им, потом сразу же в него и пишем

    int inBytePos = 7; //позиция внутри tempByte (будем идти со старшего бита к младшему)
    char tempByte = 0; //байт для записи

    while(source.read(&ch, 1)){
        auto &bits = codesMap[ch];
        //пишем биты
        int pos = 0;
        for (bool b : bits) {
            tempByte |= (b << inBytePos);
            if (inBytePos == 0) {
                dest.write(&tempByte, 1);
                tempByte = 0;
                inBytePos = 7;
            } else {
                inBytePos--;
            }
            pos++;
        }
    }

    if (inBytePos != 7) {
        dest.write(&tempByte, 1); //записали хвост
        tempByte = inBytePos;
    } else {
        tempByte = -1;
    }
    dest.write(&tempByte, 1);//последний байт всегда показывает длину "хвоста предпоследнего байта"
}

void BodyBinary::readFromCompressedFile(ifstream &source, ostream &dest, Node *fanoTree) {
    Node* curNode = fanoTree;

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
                dest.write(&toByte, 1);
                curNode = fanoTree;
            }
        }
    }
}
