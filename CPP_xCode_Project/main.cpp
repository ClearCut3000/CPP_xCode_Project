#include <iostream>
#include "EventQueue.hpp"
#include "SmartDictionary.hpp"
#include "Cache.hpp"
#include "TaskScheduler.hpp"
#include "LinkedList.hpp"
#include "CircularBuffer.hpp"
#include "LRUCache.hpp"
#include "AutoCompleteTrie.hpp"
#include "RPNCalculator.hpp"
#include "RLECompressor.hpp"
#include "MinHeap.hpp"

int main() {
  while (true) {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    std::cout << "1. Менеджер событий\n";
    std::cout << "2. Умный словарь\n";
    std::cout << "3. Кэш\n";
    std::cout << "4. Планировщик задач\n";
    std::cout << "5. Linked List\n";
    std::cout << "6. Кольцевая очередь\n";
    std::cout << "7. LRU-кэш\n";
    std::cout << "8. Автодополнение (Trie)\n";
    std::cout << "9. Калькулятор (ОПН)\n";
    std::cout << "10. Архиватор (RLE)\n";
    std::cout << "11. Мин-куча (MinHeap)\n";
    std::cout << "12. Выход\n";
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
      case 7: LRUCacheModule::run(); break;
      case 8: AutoComplete::Trie().run(); break;
      case 9: RPN::RPNCalculator().run(); break;
      case 10: {
        RLE::RLECompressor compressor;

        while (true) {
          std::cout << "\n=== Архиватор (RLE) ===\n";
          std::cout << "1. Сжать строку\n";
          std::cout << "2. Распаковать строку\n";
          std::cout << "3. Назад в меню\n";
          std::cout << "Выбор: ";

          int action;
          std::cin >> action;
          std::cin.ignore();

          if (action == 3) break;

          std::string input;
          std::cout << "Введите строку: ";
          std::getline(std::cin >> std::ws, input);

          if (action == 1) {
            // Простая проверка: если строка уже выглядит как RLE, предупредим
            bool looksCompressed = true;
            for (size_t i = 0; i < input.size();) {
              if (!isdigit(input[i])) {
                looksCompressed = false;
                break;
              }
              size_t j = i;
              while (j < input.size() && isdigit(input[j])) ++j;
              if (j == input.size() || !isalpha(input[j])) {
                looksCompressed = false;
                break;
              }
              i = j + 1;
            }

            if (looksCompressed) {
              std::cout << "⚠️ Строка уже выглядит как сжатая. Продолжить сжатие? (y/n): ";
              char confirm;
              std::cin >> confirm;
              std::cin.ignore();
              if (confirm != 'y' && confirm != 'Y') continue;
            }

            std::string compressed = compressor.compress(input);
            std::cout << "Сжатая строка: " << compressed << "\n";

          } else if (action == 2) {
            std::string decompressed = compressor.decompress(input);
            std::cout << "Результат декомпрессии: " << decompressed << "\n";

          } else {
            std::cout << "❌ Неверный выбор.\n";
          }
        }
        break;
      }
      case 11: MinHeapModule::HeapCLI::run(); break;
      case 12: std::cout << "Выход...\n"; return 0;
      default: std::cout << "Неверный выбор!\n";
    }
  }
}
