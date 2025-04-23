#include <iostream>
#include "LinkedList.hpp"

using namespace std;
using namespace LinkedListModule;

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
  while (head) {
    Node* temp = head;
    head = head->next;
    delete temp;
  }
}

void LinkedList::push_front(int value) {
  Node* newNode = new Node(value);
  newNode->next = head;
  head = newNode;
}

void LinkedList::push_back(int value) {
  Node* newNode = new Node(value);
  if (!head) {
    head = newNode;
    return;
  }
  Node* curr = head;
  while (curr->next) {
    curr = curr->next;
  }
  curr->next = newNode;
}

bool LinkedList::remove(int value) {
  Node* curr = head;
  Node* prev = nullptr;
  while (curr) {
    if (curr->data == value) {
      if (prev) prev->next = curr->next;
      else head = curr->next;
      delete curr;
      return true;
    }
    prev = curr;
    curr = curr->next;
  }
  return false;
}

Node* LinkedList::find(int value) {
  Node* curr = head;
  while (curr) {
    if (curr->data == value) return curr;
    curr = curr->next;
  }
  return nullptr;
}

void LinkedList::display() const {
  Node* curr = head;
  cout << "Список: ";
  while (curr) {
    cout << curr->data << " -> ";
    curr = curr->next;
  }
  cout << "nullptr\n";
}

void LinkedListModule::run() {
  LinkedList list;
  while (true) {
    cout << "\n=== Linked List Меню ===\n";
    cout << "1. Вставить в начало\n";
    cout << "2. Вставить в конец\n";
    cout << "3. Удалить значение\n";
    cout << "4. Найти значение\n";
    cout << "5. Показать список\n";
    cout << "6. Назад\n";
    cout << "Ваш выбор: ";

    int cmd;
    cin >> cmd;
    cin.ignore();

    int value;
    switch (cmd) {
      case 1:
        cout << "Введите значение: "; cin >> value;
        list.push_front(value);
        break;
      case 2:
        cout << "Введите значение: "; cin >> value;
        list.push_back(value);
        break;
      case 3:
        cout << "Введите значение: "; cin >> value;
        if (list.remove(value)) cout << "Удалено.\n";
        else cout << "Значение не найдено.\n";
        break;
      case 4:
        cout << "Введите значение: "; cin >> value;
        if (list.find(value)) cout << "Найдено!\n";
        else cout << "Не найдено.\n";
        break;
      case 5:
        list.display();
        break;
      case 6:
        return;
      default:
        cout << "Неверный выбор.\n";
    }
  }
}
