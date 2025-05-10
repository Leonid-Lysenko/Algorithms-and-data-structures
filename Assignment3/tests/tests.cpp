/* Leonid Lysenko st128618@student.spbu.ru
   Assignment3
*/

#include "shannon_coder.h"
#include "shannon_decoder.h"
#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <algorithm>

TEST(ShannonCodingTests, EmptyFile) {
    std::ofstream("empty_input").close();
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("empty_input", content);
    
    ASSERT_TRUE(freqs.empty());
    remove("empty_input");
}

TEST(ShannonCodingTests, SingleCharacter) {
    std::ofstream("single_input") << "a";
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("single_input", content);
    
    ASSERT_EQ(freqs.size(), 1);
    ASSERT_EQ(freqs[0].symbol, 'a');
    ASSERT_DOUBLE_EQ(freqs[0].frequency, 1.0);
    remove("single_input");
}

TEST(ShannonCodingTests, AllCharactersSame) {
    std::ofstream file("same_input");
    file << std::string(100, 'x');
    file.close();
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("same_input", content);
    
    ASSERT_EQ(freqs.size(), 1);
    ASSERT_EQ(freqs[0].symbol, 'x');
    remove("same_input");
}

TEST(ShannonCodingTests, SpecialCharacters) {
    std::string special = "!@#$%^&*()_+-=[]{}|;:',.<>/?";
    std::ofstream special_file("special_input", std::ios::binary);
    special_file << special;
    special_file.close();
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("special_input", content);
    
    // Проверка, что все символы были корректно обработаны
    ASSERT_EQ(freqs.size(), special.size());
    
    std::map<char, std::string> codes;
    for (const auto& s : freqs) codes[s.symbol] = "";
    
    GenerateShannonCodes(freqs, codes);
    SaveCompressedData(content, codes);
    DecodeCompressedFile();
    
    std::ifstream decoded("decoded", std::ios::binary);
    std::string decoded_content((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    
    ASSERT_EQ(special, decoded_content);
    
    remove("special_input");
    remove("encoded");
    remove("dictionary");
    remove("decoded");
}

TEST(ShannonCodingTests, LargeFile) {
    std::string large(100000, ' ');
    for (auto& c : large) c = 'A' + rand()%26;
    
    std::ofstream("large_input") << large;
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("large_input", content);
    ASSERT_GT(freqs.size(), 10); // Проверяем, что нашлось много разных символов
    
    std::map<char, std::string> codes;
    for (const auto& s : freqs) codes[s.symbol] = "";
    
    GenerateShannonCodes(freqs, codes);
    SaveCompressedData(content, codes);
    DecodeCompressedFile();
    
    std::ifstream decoded("decoded");
    std::string decoded_content((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    
    ASSERT_EQ(large, decoded_content);
    remove("large_input");
    remove("encoded");
    remove("dictionary");
    remove("decoded");
}

TEST(ShannonCodingTests, FrequencyCalculation) {
    std::ofstream("freq_input") << "aabbbcccc";
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("freq_input", content);
    
    ASSERT_EQ(freqs.size(), 3);
    ASSERT_EQ(freqs[0].symbol, 'c');
    ASSERT_DOUBLE_EQ(freqs[0].frequency, 4.0/9.0);
    ASSERT_EQ(freqs[1].symbol, 'b');
    ASSERT_DOUBLE_EQ(freqs[1].frequency, 3.0/9.0);
    ASSERT_EQ(freqs[2].symbol, 'a');
    ASSERT_DOUBLE_EQ(freqs[2].frequency, 2.0/9.0);
    remove("freq_input");
}

TEST(ShannonCodingTests, UniqueCodes) {
    std::string test = "abcdefghijklmnopqrstuvwxyz";
    std::ofstream("unique_input") << test;
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("unique_input", content);
    std::map<char, std::string> codes;
    for (const auto& s : freqs) codes[s.symbol] = "";
    
    GenerateShannonCodes(freqs, codes);
    
    std::vector<std::string> code_values;
    for (const auto& p : codes) code_values.push_back(p.second);
    
    // Проверяем, что все коды уникальны
    std::sort(code_values.begin(), code_values.end());
    ASSERT_EQ(std::unique(code_values.begin(), code_values.end()), code_values.end());
    remove("unique_input");
}

TEST(ShannonCodingTests, PrefixProperty) {
    std::string test = "abcdefghij";
    std::ofstream("prefix_input") << test;
    
    std::string content;
    auto freqs = CalculateSymbolFrequencies("prefix_input", content);
    std::map<char, std::string> codes;
    for (const auto& s : freqs) codes[s.symbol]="";
    
    GenerateShannonCodes(freqs, codes);
    
    // Проверяем префиксное свойство
    for (const auto& p1 : codes) {
        for (const auto& p2 : codes) {
            if (p1.first != p2.first) {
                ASSERT_FALSE(p1.second.empty());
                ASSERT_FALSE(p2.second.empty());
                ASSERT_NE(p1.second, p2.second);
                if (p1.second.size()<p2.second.size()) {
                    ASSERT_NE(p1.second, p2.second.substr(0, p1.second.size()));
                } else {
                    ASSERT_NE(p2.second, p1.second.substr(0, p2.second.size()));
                }
            }
        }
    }
    remove("prefix_input");
}
