/* Leonid Lysenko st128618@student.spbu.ru
   Assignment3
*/

#include "shannon_coder.h"
#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<SymbolInfo> CalculateSymbolFrequencies(const std::string& filename, std::string& file_content) {
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file) {
        std::cerr << "Ошибка открытия файла! Печалька..." << std::endl;
        exit(1);
    }

    // Подсчет количества каждого символа
    std::map<char, int> symbol_counts;
    char current_char;
    int total_symbols = 0;

    while (input_file.get(current_char)) {
        if (current_char != '\n') {
            symbol_counts[current_char]++;
            total_symbols++;
            file_content += current_char;
        }
    }

    // Расчет вероятностей
    std::vector<SymbolInfo> frequency_table;
    for (const auto& pair : symbol_counts) {
        frequency_table.push_back({pair.first, static_cast<double>(pair.second)/total_symbols});
    }

    // Сортировка по убыванию частот символоы
    std::sort(frequency_table.begin(), frequency_table.end(),
        [](const SymbolInfo& a, const SymbolInfo& b) {
            return a.frequency > b.frequency;
        });
    return frequency_table;
}

void SaveCompressedData(const std::string& content, const std::map<char, std::string>& encoding_table) {
    std::ofstream compressed_file("encoded", std::ios::binary);
    std::ofstream dictionary_file("dictionary");

    dictionary_file << content.length() << '\n';

    // Записываем словарь
    for (const auto& pair : encoding_table) {
        dictionary_file << pair.first << '|' << pair.second << '\n';
    }

    // Побитовая запись
    unsigned char current_byte = 0;
    int bits_written = 0;

    for (char ch : content) {
        for (char code_bit : encoding_table.at(ch)) {
            if (code_bit == '1') {
                current_byte |= (1 << (7-bits_written));
            }
            bits_written++;

            if (bits_written == 8) {
                compressed_file.put(current_byte);
                current_byte = 0;
                bits_written = 0;
            }
        }
    }
    if (bits_written > 0) {
        compressed_file.put(current_byte);
    }
}

void GenerateShannonCodes(std::vector<SymbolInfo>& symbols, std::map<char, std::string>& code_table) {
    if (symbols.size() <= 1) return;

    // Расчет суммарной вероятности	
    double total_probability = 0;
    for (const auto& symbol : symbols) {
        total_probability += symbol.frequency;
    }

    double half_probability = total_probability/2;
    double cumulative_probability = 0;
    size_t split_index = 0;

    // Разделяем символы на 2 группы
    for (; split_index < symbols.size(); ++split_index) {
        cumulative_probability += symbols[split_index].frequency;
        code_table[symbols[split_index].symbol] += "0";
        if (cumulative_probability >= half_probability) break;
    }

    for (size_t i = split_index + 1; i < symbols.size(); ++i) {
        code_table[symbols[i].symbol] += "1";
    }

    // Рекурсивно обрабатываем обе группы
    std::vector<SymbolInfo> left_group(symbols.begin(), symbols.begin()+split_index+1);
    std::vector<SymbolInfo> right_group(symbols.begin()+split_index+1, symbols.end());

    GenerateShannonCodes(left_group, code_table);
    GenerateShannonCodes(right_group, code_table);
}
