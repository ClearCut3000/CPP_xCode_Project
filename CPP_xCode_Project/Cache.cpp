#include "Cache.hpp"
#include <sstream>

namespace CacheModule {

Cache::Cache(size_t capacity) : capacity_(capacity) {}

Cache::~Cache() {
  for (Node* node : order_) {
    delete node;
  }
}

void Cache::put(int key, const std::string& value) {
  if (map_.count(key)) {
    // ключ уже существует — обновим значение
    (*map_[key])->value = value;
    return;
  }
  
  if (order_.size() >= capacity_) {
    evict();
  }
  
  Node* newNode = new Node(key, value);
  order_.push_back(newNode);
  map_[key] = --order_.end();
}

std::string* Cache::get(int key) {
  if (!map_.count(key)) return nullptr;
  return &((*map_[key])->value);
}

void Cache::evict() {
  if (order_.empty()) return;
  Node* oldest = order_.front();
  map_.erase(oldest->key);
  order_.pop_front();
  delete oldest;
}

void Cache::print() const {
  std::cout << "\n--- Кэш: ---\n";
  for (Node* node : order_) {
    std::cout << node->key << " => " << node->value << "\n";
  }
}

void run() {
  std::cout << "Кэш инициализирован. Команды: put <k> <v>, get <k>, print, exit\n";
  
  Cache cache(5); // фиксированный размер
  
  std::string command;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, command);
    std::istringstream iss(command);
    
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "put") {
      int key;
      std::string value;
      if (iss >> key >> value) {
        cache.put(key, value);
      } else {
        std::cout << "Неверный формат. Используй: put <целое_число> <строка>\n";
      }
    } else if (cmd == "get") {
      int key;
      if (iss >> key) {
        std::string* val = cache.get(key);
        if (val) {
          std::cout << "Значение: " << *val << '\n';
        } else {
          std::cout << "Значение не найдено\n";
        }
      } else {
        std::cout << "Неверный формат. Используй: get <целое_число>\n";
      }
    } else if (cmd == "print") {
      cache.print();
    } else if (cmd == "exit") {
      break;
    } else {
      std::cout << "Неизвестная команда.\n";
    }
  }
}


}
