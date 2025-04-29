#include <iostream>
#include <stdexcept>
#include "CircularBuffer.hpp"

using namespace std;
using namespace CircularBufferModule;

CircularBuffer::CircularBuffer(int capacity)
  : cap(capacity), count(0), head(0), tail(0) {
  buffer = new int[cap];
}

CircularBuffer::~CircularBuffer() {
  delete[] buffer;
}

void CircularBuffer::push(int value) {
  buffer[tail] = value;
  tail = (tail + 1) % cap;
  if (count == cap) {
    head = (head + 1) % cap; // перезапись: сдвигаем head
  } else {
    ++count;
  }
}

void CircularBuffer::pop() {
  if (empty()) {
    throw runtime_error("Попытка pop() из пустого буфера");
  }
  head = (head + 1) % cap;
  --count;
}

int CircularBuffer::front() const {
  if (empty()) {
    throw runtime_error("Попытка front() из пустого буфера");
  }
  return buffer[head];
}

int CircularBuffer::back() const {
  if (empty()) {
    throw runtime_error("Попытка back() из пустого буфера");
  }
  return buffer[(tail - 1 + cap) % cap];
}

int CircularBuffer::size() const {
  return count;
}

bool CircularBuffer::empty() const {
  return count == 0;
}

void CircularBuffer::display() const {
  cout << "Буфер: ";
  for (int i = 0; i < count; ++i) {
    int idx = (head + i) % cap;
    cout << buffer[idx] << " ";
  }
  cout << "\n";
}

void CircularBufferModule::run() {
  int capacity;
  cout << "Введите размер буфера: ";
  cin >> capacity;

  CircularBuffer buf(capacity);

  while (true) {
    cout << "\n=== Circular Buffer Меню ===\n";
    cout << "1. Push\n";
    cout << "2. Pop\n";
    cout << "3. Front\n";
    cout << "4. Back\n";
    cout << "5. Display\n";
    cout << "6. Размер\n";
    cout << "7. Пуст ли?\n";
    cout << "8. Назад\n";
    cout << "Ваш выбор: ";

    int cmd;
    cin >> cmd;
    int val;

    try {
      switch (cmd) {
        case 1:
          cout << "Введите значение: "; cin >> val;
          buf.push(val);
          break;
        case 2:
          buf.pop();
          cout << "Удален первый элемент.\n";
          break;
        case 3:
          cout << "Front: " << buf.front() << "\n";
          break;
        case 4:
          cout << "Back: " << buf.back() << "\n";
          break;
        case 5:
          buf.display();
          break;
        case 6:
          cout << "Размер: " << buf.size() << "\n";
          break;
        case 7:
          cout << (buf.empty() ? "Да, пуст.\n" : "Нет, не пуст.\n");
          break;
        case 8:
          return;
        default:
          cout << "Неверный выбор.\n";
      }
    } catch (const exception& e) {
      cout << "Ошибка: " << e.what() << "\n";
    }
  }
}
