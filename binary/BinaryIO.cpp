#include "BinaryIO.h"

char BinaryIO::readByte(std::ifstream &source, int &chPos, char &ch) {
    int inNumPos = 7; //позиция для байта, куда пишем
    char readedByte = 0;
    while (inNumPos >= 0) {

        while (chPos >= 0 && inNumPos >= 0) {
            unsigned char bit = (ch & (1 << chPos)); // взяли бит
            int seek = inNumPos - chPos; //смотрим куда двигать бит, чтобы занести в tempByte
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
            chPos = 7;
        }
    }
    return readedByte;
}

void BinaryIO::writeByte(std::ostream &out, char byte, char &usedByte, int &inBytePos) {
    int inNumPos = 7;
    while (inNumPos >= 0) {
        while (inBytePos >= 0 && inNumPos >= 0) {
            unsigned char bit = (byte & (1 << inNumPos)); // взяли бит
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
