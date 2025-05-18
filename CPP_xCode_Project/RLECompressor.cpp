#include "RLECompressor.hpp"
#include <sstream>
#include <cctype>

/*
 AAABBC -compress-> 3A2B1C
 3A2B1C -decompress-> AAABBC
 */

namespace RLE {

std::string RLECompressor::compress(const std::string& input) {
  if (input.empty()) return "";
  
  std::ostringstream result;
  char current = input[0];
  int count = 1;
  
  for (size_t i = 1; i < input.size(); ++i) {
    if (input[i] == current) {
      ++count;
    } else {
      result << count << current;
      current = input[i];
      count = 1;
    }
  }
  
  result << count << current;
  return result.str();
}

std::string RLECompressor::decompress(const std::string& input) {
  std::ostringstream result;
  std::string countStr;
  
  for (char ch : input) {
    if (std::isdigit(ch)) {
      countStr += ch;
    } else {
      if (countStr.empty()) return "Ошибка: некорректный формат (отсутствует количество перед символом).";
      int count = std::stoi(countStr);
      result << std::string(count, ch);
      countStr.clear();
    }
  }
  
  if (!countStr.empty()) return "Ошибка: строка оканчивается числом без символа.";
  
  return result.str();
}

} // namespace RLE
