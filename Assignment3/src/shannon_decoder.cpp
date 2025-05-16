/* Leonid Lysenko st128618@student.spbu.ru
   Assignment3
*/

#include "shannon_decoder.h"
#include <fstream>
#include <iostream>

void DecodeCompressedFile() {
    std::ifstream dictionary_file("dictionary");
    std::ifstream encoded_file("encoded", std::ios::binary);
    std::ofstream decoded_file("decoded");

    std::map<std::string, char> decoding_map;
    std::string dictionary_line;
    std::getline(dictionary_file, dictionary_line);
    const size_t expected_symbols = std::stoul(dictionary_line);

    // Читаем словарь построчно
    while (std::getline(dictionary_file, dictionary_line)) {
        char symbol = dictionary_line[0];
        std::string code = dictionary_line.substr(2);
        decoding_map[code] = symbol;
    }

    // Декодинг с побитовым чтением
    std::string accumulated_bits;
    size_t symbols_decoded = 0;
    unsigned char input_byte;

    while (encoded_file.read(reinterpret_cast<char*>(&input_byte), 1)) {
        for (int bit_position = 7; bit_position >= 0; --bit_position) {
            accumulated_bits += (input_byte & (1 << bit_position)) ? '1' : '0';
            auto code_match = decoding_map.find(accumulated_bits);
            if (code_match != decoding_map.end()) {
                decoded_file << code_match->second;
                accumulated_bits.clear();
                symbols_decoded++;

                if (symbols_decoded == expected_symbols) {
                    return;
                }
            }
        }
    }
}
