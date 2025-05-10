/* Leonid Lysenko st128618@student.spbu.ru
   Assignment3
*/

#ifndef SHANNON_CODER_H
#define SHANNON_CODER_H

#include <vector>
#include <map>
#include <string>

// Хранение символа и его вероятности
struct SymbolInfo {
    char symbol;
    double frequency;
};

// Ф-ия для чтения исходного файла и подсчета частот симвооов
std::vector<SymbolInfo> CalculateSymbolFrequencies(const std::string& filename, std::string& file_content);

// Ф-ия для записи закодированного файла и словаря
void SaveCompressedData(const std::string& content, const std::map<char, std::string>& encoding_table);

// Рекурсивная функция для построения кодов Шеннона
void GenerateShannonCodes(std::vector<SymbolInfo>& symbols, std::map<char, std::string>& code_table);

#endif
