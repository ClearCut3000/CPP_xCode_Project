#include "MinHeap.hpp"
#include <sstream>

using namespace MinHeapModule;

void HeapCLI::run() {
  MinHeap heap;
  while (true) {
    std::cout << "\n=== Минимальная куча ===\n";
    std::cout << "1. Вставить элемент\n";
    std::cout << "2. Извлечь минимум\n";
    std::cout << "3. Показать минимум\n";
    std::cout << "4. Размер кучи\n";
    std::cout << "5. Пустая ли куча?\n";
    std::cout << "6. Показать содержимое\n";
    std::cout << "7. Показать дерево\n";
    std::cout << "8. Heapify по массиву\n";
    std::cout << "9. Назад\n";
    std::cout << "Выбор: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    try {
      if (choice == 1) {
        std::cout << "Введите число: ";
        int value;
        std::cin >> value;
        heap.insert(value);
        std::cout << "✅ Вставлено.\n";
      } else if (choice == 2) {
        std::cout << "🧺 Извлечено: " << heap.extract_min() << "\n";
      } else if (choice == 3) {
        std::cout << "🔍 Минимум: " << heap.peek() << "\n";
      } else if (choice == 4) {
        std::cout << "📦 Размер: " << heap.size() << "\n";
      } else if (choice == 5) {
        std::cout << (heap.empty() ? "✅ Куча пуста.\n" : "❌ Куча не пуста.\n");
      } else if (choice == 6) {
        heap.display();
      } else if (choice == 7) {
        heap.display_tree();
      } else if (choice == 8) {
        std::cout << "Введите числа через пробел: ";
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::vector<int> values;
        int x;
        while (iss >> x) values.push_back(x);
        heap.heapify(values);
        std::cout << "✅ Куча построена из массива.\n";
      } else if (choice == 9) break;
      else {
        std::cout << "❌ Неверный выбор.\n";
      }
    } catch (const std::exception& e) {
      std::cout << "⚠️ Ошибка: " << e.what() << "\n";
    }
  }
}
