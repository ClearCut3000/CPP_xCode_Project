#include "MinHeap.hpp"
#include <stdexcept>
#include <iomanip>
#include <cmath>

using namespace MinHeapModule;

MinHeap::MinHeap(int initialCapacity) : capacity(initialCapacity), count(0) {
  heap = new int[capacity];
}

MinHeap::~MinHeap() {
  delete[] heap;
}

void MinHeap::resize() {
  capacity *= 2;
  int* newHeap = new int[capacity];
  for (int i = 0; i < count; ++i) newHeap[i] = heap[i];
  delete[] heap;
  heap = newHeap;
}

void MinHeap::heapify_up(int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (heap[parent] > heap[index]) {
      std::swap(heap[parent], heap[index]);
      index = parent;
    } else break;
  }
}

void MinHeap::heapify_down(int index) {
  while (true) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < count && heap[left] < heap[smallest]) smallest = left;
    if (right < count && heap[right] < heap[smallest]) smallest = right;

    if (smallest != index) {
      std::swap(heap[index], heap[smallest]);
      index = smallest;
    } else break;
  }
}

void MinHeap::insert(int value) {
  if (count == capacity) resize();
  heap[count++] = value;
  heapify_up(count - 1);
}

int MinHeap::extract_min() {
  if (count == 0) throw std::underflow_error("Heap is empty");
  int minValue = heap[0];
  heap[0] = heap[--count];
  heapify_down(0);
  return minValue;
}

int MinHeap::peek() const {
  if (count == 0) throw std::underflow_error("Heap is empty");
  return heap[0];
}

int MinHeap::size() const { return count; }

bool MinHeap::empty() const { return count == 0; }

void MinHeap::display() const {
  std::cout << "MinHeap [ ";
  for (int i = 0; i < count; ++i) std::cout << heap[i] << " ";
  std::cout << "]\n";
}

std::vector<int> MinHeap::toVector() const {
  return std::vector<int>(heap, heap + count);
}

void MinHeap::heapify(const std::vector<int>& data) {
  if ((int)data.size() > capacity) {
    delete[] heap;
    capacity = data.size() * 2;
    heap = new int[capacity];
  }
  count = data.size();
  for (int i = 0; i < count; ++i) {
    heap[i] = data[i];
  }
  for (int i = count / 2 - 1; i >= 0; --i) {
    heapify_down(i);
  }
}

void MinHeap::display_tree() const {
  std::cout << "\n📐 Heap Tree (уровни):\n";
  int level = 0, index = 0;
  while (index < count) {
    int items = std::min(1 << level, count - index);
    for (int i = 0; i < items; ++i) {
      std::cout << std::setw(3) << heap[index + i] << " ";
    }
    std::cout << "\n";
    index += items;
    ++level;
  }
}
