#include "Archive.h"
#include "utils/Log.h"
#include "archiver/Tree.h"
#include "archiver/FanoCodesBinary.h"
#include "archiver/BodyBinary.h"

#include <sstream>

using namespace std;

Archive::Archive(std::ifstream &input) : input(input) {
}

void Archive::compress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    // Считывание потока данных с файла
    // Заполнение storedUsages, подсчет количества использований
    auto codes = FanoCodesBinary::makeFromFile(input);

    Log::v("Generating keys...");
    // Генератор keys
    FanoCodesBinary::writeToFile(output, codes);

    Log::v("Generating data stream...");

    // Создание archive
    input.clear();
    input.seekg(0);
    BodyBinary::writeCompressedToFile(input, output, codes);
    Log::v("File successfully archived!");
}

void Archive::decompress(std::ofstream &output) {
    std::ios_base::sync_with_stdio(false);

    Log::v("Generating tree by stream data...");
    Node treeHead = FanoCodesBinary::readFromFile(input);

    Log::v("Converting stream data to export file...");

    BodyBinary::readFromCompressedFile(input, output, &treeHead);

    Log::v("File successfully unpacked!");
}