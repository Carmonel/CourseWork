#include "ArchiverExecutor.h"

#include <iostream>
#include <string>

#include "Fano.h"

using namespace std;

#include "utils/Exception.h"
#include "Archive.h"
#include "utils/Utils.h"
#include "utils/Log.h"

void checkFile(ofstream &fs, const char* msg){
    if (fs.fail() || !fs.is_open()){
        fs.close();
        throw FileOpenError(msg);
    }
}

void ArchiverExecutor::execute(Config &config) {

    std::ifstream inputBinaryFileStream(config.path, std::ios::binary);
    if (inputBinaryFileStream.fail() || !inputBinaryFileStream.is_open()){
        inputBinaryFileStream.close();
        throw FileOpenError("Can't open input file!!!");
    }

    if (config.mode == Config::COMPRESS) {
        std::ofstream outputBinaryFileStream(Utils::createCompressedFileName(config.path), std::ios::binary);
        checkFile(outputBinaryFileStream, "Can't open output file!!!");

        Log::v("Working with '" + config.path + "' and '" + Utils::createCompressedFileName(config.path) + "'");
        Archive(inputBinaryFileStream).compress(outputBinaryFileStream);

        inputBinaryFileStream.close();
        outputBinaryFileStream.close();
    } else if (config.mode == Config::DECOMPRESS) {
        std::ofstream outputBinaryFileStream(Utils::createDecompressedFileName(config.path), std::ios::binary);
        checkFile(outputBinaryFileStream, "Can't open output file!!!");

        if (!outputBinaryFileStream.fail()) outputBinaryFileStream.clear();//?????

        Log::v("Working with '" + config.path + "' and '" + Utils::createDecompressedFileName(config.path) + "'");
        Archive(inputBinaryFileStream).decompress(outputBinaryFileStream);

        inputBinaryFileStream.close();
        outputBinaryFileStream.close();
    } else {
        throw WrongArchiverMode("No execute mode!!!");
    }
}
