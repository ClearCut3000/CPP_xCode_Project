#ifndef CIRCULARBUFFER_HPP
#define CIRCULARBUFFER_HPP

namespace CircularBufferModule {

class CircularBuffer {
public:
  CircularBuffer(int capacity);
  ~CircularBuffer();

  void push(int value);         // Добавить элемент (перезаписывает при переполнении)
  void pop();                   // Удалить самый старый
  int front() const;           // Первый элемент
  int back() const;            // Последний элемент
  int size() const;
  bool empty() const;

  void display() const;        // Для отладки
private:
  int* buffer;
  int cap;
  int count;
  int head;
  int tail;
};

void run();

} // namespace CircularBufferModule

#endif // CIRCULARBUFFER_HPP
