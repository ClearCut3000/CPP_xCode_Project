/*
  Реализация LRU-кэша (Least Recently Used) на C++.

   Общая идея:
  Кэш с политикой "наименее недавно использованного" хранит элементы фиксированного объёма
  и автоматически удаляет наименее использованный (доступный) элемент при переполнении.
  Это полезно, например, для веб-браузеров, баз данных, операционных систем и любых ограниченных
  по памяти подсистем хранения временных данных.

   Основные операции:
    - put(key, value): добавить или обновить значение по ключу. Если кэш полон — удалить самый "старый".
    - get(key): получить значение по ключу и пометить его как "наиболее недавно использованный".
    - display(): отобразить содержимое кэша в порядке от самого свежего к самому старому.

   Структуры данных:
    1. std::list<std::pair<K, V>> items
       - Двунаправленный список, где каждый элемент — это пара (ключ, значение).
       - Новый элемент добавляется в начало списка (спереди) как "самый свежий".
       - Зад списка содержит "самый старый" элемент, который удаляется при переполнении.

    2. std::unordered_map<K, list::iterator> map
       - Хеш-таблица, сопоставляющая ключи итераторам в списке `items`.
       - Обеспечивает доступ к нужному узлу списка за O(1), что критично для скорости.

   Принцип работы:
    - При вызове put():
        • Если ключ уже существует: удаляем старую позицию в списке и вставляем новую в начало.
        • Если ключ новый:
            ◦ Проверяем переполнение. Если кэш полон — удаляем последний элемент в списке (самый старый) и его запись в хеш-таблице.
            ◦ Вставляем новый элемент в начало списка и в хеш.

    - При вызове get():
        • Если ключ найден: извлекаем значение, удаляем текущую позицию из списка, и вставляем её в начало как "самую свежую".
        • Если ключ не найден — возвращаем false.

   Все основные операции (put, get) работают за O(1), благодаря сочетанию list + unordered_map.

   Замечания:
    - Используется шаблонный класс LRUCache<K, V> — позволяет использовать любые типы ключей и значений.
    - Весь код расположен в заголовочном файле (header-only), так как шаблоны должны быть определены при компиляции.

 Версия: 1.0
  Совместимость: C++11 и выше
*/

#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <iostream>
#include <unordered_map>
#include <list>

namespace LRUCacheModule {

template<typename K, typename V>
class LRUCache {
private:
    size_t capacity;

    // Список: перед — самый свежий, зад — самый старый
    std::list<std::pair<K, V>> items;

    // Хеш: ключ -> итератор в списке
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map;

public:
    LRUCache(size_t cap) : capacity(cap) {}

    void put(const K& key, const V& value) {
        auto it = map.find(key);

        if (it != map.end()) {
            // Обновляем значение и перемещаем в начало
            items.erase(it->second);
            items.push_front({key, value});
            map[key] = items.begin();
        } else {
            // Новый элемент
            if (items.size() >= capacity) {
                // Удаляем самый старый
                auto last = items.back();
                map.erase(last.first);
                items.pop_back();
            }
            items.push_front({key, value});
            map[key] = items.begin();
        }
    }

    bool get(const K& key, V& value) {
        auto it = map.find(key);
        if (it == map.end()) return false;

        // Перемещаем в начало
        auto node = *(it->second);
        items.erase(it->second);
        items.push_front(node);
        map[key] = items.begin();

        value = node.second;
        return true;
    }

    void display() const {
        std::cout << "Кэш (свежие -> старые):\n";
        for (const auto& [k, v] : items) {
            std::cout << k << ": " << v << "\n";
        }
    }

    void run() {
        LRUCache<std::string, std::string> cache(3);
        std::string cmd, key, value;

        std::cout << "LRU-кэш. Команды: put key val | get key | show | exit\n";

        while (true) {
            std::cout << "> ";
            std::cin >> cmd;

            if (cmd == "put") {
                std::cin >> key >> value;
                put(key, value);
            } else if (cmd == "get") {
                std::cin >> key;
                if (get(key, value))
                    std::cout << "Найдено: " << value << "\n";
                else
                    std::cout << "Ключ не найден\n";
            } else if (cmd == "show") {
                display();
            } else if (cmd == "exit") {
                break;
            } else {
                std::cout << "Неизвестная команда\n";
            }
        }
    }
};

inline void run() {
    LRUCache<std::string, std::string> cache(3);
    cache.run();
}

} // namespace LRUCacheModule

#endif
