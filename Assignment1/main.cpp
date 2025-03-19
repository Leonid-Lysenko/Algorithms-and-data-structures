// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "encoder.h"
#include "decoder.h"

int ascii85ToolMain(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " encode|decode input_file [output_file|--stdout]\n";
        return 1;
    }

    std::string operationMode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = (argc >= 4 && std::string(argv[3]) != "--stdout") ? argv[3] : "";

    bool outputToStdout = (argc >= 4 && std::string(argv[3]) == "--stdout");

    std::ifstream inputFileStream(inputFile, std::ios::binary);
    if (!inputFileStream) {
        std::cerr << "Ошибка открытия файла ввода.\n";
        return 1;
    }

    std::ostream* outputStream = &std::cout;
    std::ofstream outputFileStream;
    if (!outputToStdout) {
        if (outputFile.empty()) outputFile = "output.txt";
        outputFileStream.open(outputFile, std::ios::binary);
        if (!outputFileStream) {
            std::cerr << "Ошибка открытия файла вывода.\n";
            return 1;
        }
        outputStream = &outputFileStream;
    }

    try {
        if (operationMode == "encode") {
            ascii85Encoder(inputFileStream, *outputStream);
        } else if (operationMode == "decode") {
            ascii85Decoder(inputFileStream, *outputStream);
        } else {
            std::cerr << "Недопустимый режим. Используйте 'encode' или 'decode'.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    inputFileStream.close();
    if (outputFileStream.is_open()) outputFileStream.close();

    return 0;
}

int main(int argc, char* argv[]) {
    return ascii85ToolMain(argc, argv);
}

