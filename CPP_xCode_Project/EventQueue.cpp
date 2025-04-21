#include "EventQueue.hpp"
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

namespace EventManager {

struct Event {
  int id;
  std::string message;
  std::chrono::time_point<std::chrono::steady_clock> time;
};

std::queue<Event> eventQueue;
std::mutex queueMutex;
std::condition_variable queueCondVar;
std::atomic<bool> stopRequested{false};

void producer(int intervalMs) {
  int counter = 0;
  while (!stopRequested) {
    Event evt{counter++, "Event generated", std::chrono::steady_clock::now()};
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      eventQueue.push(evt);
    }
    queueCondVar.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
  }
}

void consumer() {
  using namespace std::chrono;
  while (!stopRequested) {
    std::unique_lock<std::mutex> lock(queueMutex);
    queueCondVar.wait(lock, [] { return !eventQueue.empty() || stopRequested; });

    while (!eventQueue.empty()) {
      Event evt = eventQueue.front();
      eventQueue.pop();
      lock.unlock();

      auto latency = duration_cast<milliseconds>(steady_clock::now() - evt.time).count();
      std::cout << "Обработано: ID=" << evt.id << " Msg=" << evt.message << " Latency=" << latency << "ms\n";

      lock.lock();
    }
  }
}

void run() {
  stopRequested = false;
  std::thread prod(producer, 1000);
  std::thread cons(consumer);

  std::cout << "Менеджер событий запущен. Нажмите Enter для остановки...\n";
  std::cin.get();

  stopRequested = true;
  queueCondVar.notify_all();
  prod.join();
  cons.join();

  std::cout << "Менеджер событий завершён.\n";
}
}
