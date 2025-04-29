#include <iostream>
#include "EventQueue.hpp"
#include "SmartDictionary.hpp"
#include "Cache.hpp"
#include "TaskScheduler.hpp"
#include "LinkedList.hpp"
#include "CircularBuffer.hpp"


int main() {
  while (true) {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    std::cout << "1. Менеджер событий\n";
    std::cout << "2. Умный словарь\n";
    std::cout << "3. Кэш\n";
    std::cout << "4. Планировщик задач\n";
    std::cout << "5. Linked List\n";
    std::cout << "6. Кольцевая очередь\n";
    std::cout << "7. Выход\n";
    std::cout << "Выберите номер проекта: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
      case 1: EventManager::run(); break;
      case 2: Dictionary::run(); break;
      case 3: CacheModule::run(); break;
      case 4: TaskScheduler::run(); break;
      case 5: LinkedListModule::run(); break;
      case 6: CircularBufferModule::run(); break;
      case 7: std::cout << "Выход...\n"; return 0;
      default: std::cout << "Неверный выбор!\n";
    }
  }
}
