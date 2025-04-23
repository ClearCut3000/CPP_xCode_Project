#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <iostream>

namespace CacheModule {

class Cache {
public:
    Cache(size_t capacity);
    ~Cache();

    void put(int key, const std::string& value);
    std::string* get(int key);
    void print() const;

private:
    struct Node {
        int key;
        std::string value;
        Node(int k, const std::string& v) : key(k), value(v) {}
    };

    size_t capacity_;
    std::unordered_map<int, std::list<Node*>::iterator> map_;
    std::list<Node*> order_; // список узлов в порядке вставки

    void evict();
};

void run();

}
