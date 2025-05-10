/* Leonid Lysenko st128618@student.spbu.ru
   Assignment3
*/

#include "shannon_coder.h"
#include "shannon_decoder.h"

int main() {
    std::string original_text;
    std::vector<SymbolInfo> symbol_frequencies = CalculateSymbolFrequencies("input", original_text);
    
    std::map<char, std::string> encoding_dictionary;
    for (const auto& symbol : symbol_frequencies) {
        encoding_dictionary[symbol.symbol] = "";
    }
    
    GenerateShannonCodes(symbol_frequencies, encoding_dictionary);
    SaveCompressedData(original_text, encoding_dictionary);
    
    DecodeCompressedFile();
    
    return 0;
}	
