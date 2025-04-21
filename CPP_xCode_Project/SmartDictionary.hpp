#ifndef SMART_DICTIONARY_HPP
#define SMART_DICTIONARY_HPP

#include <vector>
#include <string>

namespace Dictionary {

struct WordEntry {
  std::string word;
  std::string translation;
};

void run();

void printMenu();
void addWord(std::vector<WordEntry>& dict, int& count);
void findTranslation(const std::vector<WordEntry>& dict);
void showAllWords(const std::vector<WordEntry>& dict);
void saveToFile(const std::vector<WordEntry>& dict, const std::string& filename);
void loadFromFile(std::vector<WordEntry>& dict, int& count, const std::string& filename);

}

#endif
