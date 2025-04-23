#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

namespace LinkedListModule {

struct Node {
  int data;
  Node* next;
  Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
public:
  LinkedList();
  ~LinkedList();

  void push_front(int value);
  void push_back(int value);
  bool remove(int value);
  Node* find(int value);
  void display() const;

private:
  Node* head;
};

void run();

}

#endif // LINKEDLIST_HPP
