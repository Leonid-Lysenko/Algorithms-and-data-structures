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

    // Читаем словарь построчно
    while (std::getline(dictionary_file, dictionary_line)) {
        char symbol = dictionary_line[0];
        std::string code = dictionary_line.substr(2);
        decoding_map[code] = symbol;
    }

    // Декодинг
    std::string current_code;
    char bit;
    while (encoded_file.get(bit)) {
        current_code += bit;
        if (decoding_map.count(current_code)) {
            decoded_file << decoding_map[current_code];
            current_code.clear();
        }
    }
}
