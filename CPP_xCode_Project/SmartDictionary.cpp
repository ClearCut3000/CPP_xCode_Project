#include "SmartDictionary.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace Dictionary {

void printMenu() {
  std::cout << "\n--- МЕНЮ ---\n"
  << "1. Добавить слово\n"
  << "2. Найти перевод\n"
  << "3. Показать все слова\n"
  << "4. Выход\n";
}

void addWord(std::vector<WordEntry>& dict, int& count) {
  WordEntry entry;
  std::cout << "Введите слово: ";
  std::getline(std::cin, entry.word);
  std::cout << "Введите перевод: ";
  std::getline(std::cin, entry.translation);
  dict.push_back(entry);
  ++count;
  std::cout << "Слово добавлено!\n";
}

void findTranslation(const std::vector<WordEntry>& dict) {
  std::string query;
  std::cout << "Введите слово для поиска: ";
  std::getline(std::cin, query);
  
  auto lower = [](std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
  };
  
  query = lower(query);
  for (const auto& entry : dict) {
    if (lower(entry.word) == query) {
      std::cout << "Перевод: " << entry.translation << "\n";
      return;
    }
  }
  std::cout << "Слово не найдено.\n";
}

void showAllWords(const std::vector<WordEntry>& dict) {
  std::cout << "\n--- Все слова ---\n";
  for (const auto& entry : dict) {
    std::cout << entry.word << " - " << entry.translation << "\n";
  }
}

void saveToFile(const std::vector<WordEntry>& dict, const std::string& filename) {
  std::ofstream out(filename);
  for (const auto& entry : dict) {
    out << entry.word << '\t' << entry.translation << '\n';
  }
}

void loadFromFile(std::vector<WordEntry>& dict, int& count, const std::string& filename) {
  std::ifstream in(filename);
  if (!in) return;
  WordEntry entry;
  while (std::getline(in, entry.word, '\t') && std::getline(in, entry.translation)) {
    dict.push_back(entry);
    ++count;
  }
}

void run() {
  std::vector<WordEntry> dictionary;
  int wordCount = 0;
  const std::string filename = "dictionary.txt";
  loadFromFile(dictionary, wordCount, filename);
  
  bool running = true;
  while (running) {
    printMenu();
    int choice;
    std::cout << "Выберите опцию: ";
    std::cin >> choice;
    std::cin.ignore();
    
    switch (choice) {
      case 1: addWord(dictionary, wordCount); break;
      case 2: findTranslation(dictionary); break;
      case 3: showAllWords(dictionary); break;
      case 4: running = false; break;
      default: std::cout << "Неверный выбор.\n";
    }
  }
  
  saveToFile(dictionary, filename);
  std::cout << "Сессия завершена. Добавлено слов: " << wordCount << "\n";
}

}
