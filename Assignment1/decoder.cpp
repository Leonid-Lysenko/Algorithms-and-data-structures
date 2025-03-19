// decoder.cpp
#include "decoder.h"
#include <vector>
#include <stdexcept>
#include <iostream>

void ascii85Decoder(std::istream& inputData, std::ostream& outputData) {
    std::vector<char> symbolBuffer;
    char currentSymbol;

    while (inputData.get(currentSymbol)) {
        if (currentSymbol == '~' && inputData.peek() == '>') {
            inputData.get();
            break;
        }

        if (currentSymbol < 33 || currentSymbol > 117) {
            std::cerr << "Invalid symbol encountered: " << currentSymbol << std::endl;
            throw std::runtime_error("Недопустимый символ в кодировке ASCII85");
        }

        symbolBuffer.push_back(currentSymbol);

        if (symbolBuffer.size() == 5) {
            unsigned int decodedValue = 0;
            for (char symbol : symbolBuffer) {
                decodedValue = decodedValue * 85 + (symbol - 33);
            }

            char decodedBytes[4] = {
                static_cast<char>((decodedValue >> 24) & 0xFF),
                static_cast<char>((decodedValue >> 16) & 0xFF),
                static_cast<char>((decodedValue >> 8) & 0xFF),
                static_cast<char>(decodedValue & 0xFF)
            };

            outputData.write(decodedBytes, 4);
            symbolBuffer.clear();
        }
    }

    if (!symbolBuffer.empty()) {
        if (symbolBuffer.size() < 2 || symbolBuffer.size() > 4) {
            throw std::runtime_error("Некорректный размер последнего блока в кодировке ASCII85");
        }

        int numSymbols = symbolBuffer.size();
        while (symbolBuffer.size() < 5) {
            symbolBuffer.push_back('u');
        }

        unsigned int decodedValue = 0;
        for (char symbol : symbolBuffer) {
            decodedValue = decodedValue * 85 + (symbol - 33);
        }

        char decodedBytes[4] = {
            static_cast<char>((decodedValue >> 24) & 0xFF),
            static_cast<char>((decodedValue >> 16) & 0xFF),
            static_cast<char>((decodedValue >> 8) & 0xFF),
            static_cast<char>(decodedValue & 0xFF)
        };

        outputData.write(decodedBytes, numSymbols - 1);
    }
}
