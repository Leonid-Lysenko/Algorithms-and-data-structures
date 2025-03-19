// encoder.cpp
#include "encoder.h"
#include <vector>
#include <iostream>

void ascii85Encoder(std::istream& inputData, std::ostream& outputData) {
    std::vector<char> byteBuffer(4);

    while (inputData.read(byteBuffer.data(), 4) || inputData.gcount() > 0) {
        std::streamsize bytesRead = inputData.gcount();
        int paddingSize = 4 - bytesRead;

        for (int i = bytesRead; i < 4; ++i) byteBuffer[i] = 0;

        unsigned int encodedValue = (static_cast<unsigned char>(byteBuffer[0]) << 24) |
                                    (static_cast<unsigned char>(byteBuffer[1]) << 16) |
                                    (static_cast<unsigned char>(byteBuffer[2]) << 8) |
                                    static_cast<unsigned char>(byteBuffer[3]);

        char encodedSymbols[5];
        for (int i = 4; i >= 0; --i) {
            encodedSymbols[i] = encodedValue % 85 + 33;
            encodedValue /= 85;
        }

        outputData.write(encodedSymbols, 5 - paddingSize);
    }

    outputData << "~>";
}

