#include <iostream>
#include "EventQueue.hpp"
#include "SmartDictionary.hpp"

int main() {
  while (true) {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    std::cout << "1. Менеджер событий\n";
    std::cout << "2. Умный словарь\n";
    std::cout << "3. Выход\n";
    std::cout << "Выберите номер проекта: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
      case 1: EventManager::run(); break;
      case 2: Dictionary::run(); break;
      case 3: std::cout << "Выход...\n"; return 0;
      default: std::cout << "Неверный выбор!\n";
    }
  }
}
