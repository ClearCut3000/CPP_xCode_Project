#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <iostream>
#include <vector>

namespace MinHeapModule {

class MinHeap {
private:
  int* heap;
  int capacity;
  int count;
  
  void resize();
  void heapify_up(int index);
  void heapify_down(int index);
  
public:
  MinHeap(int initialCapacity = 16);
  ~MinHeap();
  
  void insert(int value);
  int extract_min();
  int peek() const;
  
  int size() const;
  bool empty() const;
  
  void display() const;
  void display_tree() const;
  
  void heapify(const std::vector<int>& data);
  std::vector<int> toVector() const;
};

class HeapCLI {
public:
  static void run();
};

} // namespace MinHeapModule

#endif // MIN_HEAP_HPP
