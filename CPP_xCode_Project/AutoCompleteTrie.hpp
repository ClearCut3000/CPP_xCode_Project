#ifndef AUTOCOMPLETE_TRIE_HPP
#define AUTOCOMPLETE_TRIE_HPP

#include <iostream>
#include <vector>
#include <string>

namespace AutoComplete {

class TrieNode {
public:
    TrieNode* children[26];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; ++i) children[i] = nullptr;
    }
};

class Trie {
private:
    TrieNode* root;

    void collectWords(TrieNode* node, std::string current, std::vector<std::string>& result) {
        if (node->isEndOfWord) result.push_back(current);
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                collectWords(node->children[i], current + static_cast<char>('a' + i), result);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        clear(root);
    }

    void clear(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i)
            clear(node->children[i]);
        delete node;
    }

    void addWord(const std::string& word) {
        TrieNode* current = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!current->children[idx]) {
                current->children[idx] = new TrieNode();
            }
            current = current->children[idx];
        }
        current->isEndOfWord = true;
    }

    std::vector<std::string> searchPrefix(const std::string& prefix) {
        TrieNode* current = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!current->children[idx]) {
                return {};  // префикс не найден
            }
            current = current->children[idx];
        }

        std::vector<std::string> results;
        collectWords(current, prefix, results);
        return results;
    }

    void run() {
        Trie trie;
        std::string command, word;

        while (true) {
            std::cout << "\n1. Добавить слово\n";
            std::cout << "2. Найти автодополнения по префиксу\n";
            std::cout << "3. Выход\n";
            std::cout << "Выбор: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    std::cout << "Введите слово (строчные буквы): ";
                    std::cin >> word;
                    trie.addWord(word);
                    std::cout << "Слово добавлено.\n";
                    break;
                case 2:
                    std::cout << "Введите префикс: ";
                    std::cin >> word;
                    {
                        auto results = trie.searchPrefix(word);
                        if (results.empty()) {
                            std::cout << "Нет совпадений.\n";
                        } else {
                            std::cout << "Найдено:\n";
                            for (const auto& w : results) {
                                std::cout << " - " << w << "\n";
                            }
                        }
                    }
                    break;
                case 3:
                    std::cout << "Завершение.\n";
                    return;
                default:
                    std::cout << "Неверный ввод.\n";
            }
        }
    }
};

} // namespace AutoComplete

#endif // AUTOCOMPLETE_TRIE_HPP
