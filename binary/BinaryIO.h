#ifndef COURSEWORK_BINARYIO_H
#define COURSEWORK_BINARYIO_H

#include <fstream>

class BinaryIO {
public:
    static char readByte(std::ifstream &source, int &chPos, char &ch);

    static void writeByte(std::ostream &out, char byte, char &usedByte, int &inBytePos);
};


#endif //COURSEWORK_BINARYIO_H
