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
    std::chrono::steady_clock::time_point timestamp;

    Event(int i, const std::string& msg, std::chrono::steady_clock::time_point ts)
        : id(i), message(msg), timestamp(ts) {}
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
    while (!stopRequested) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondVar.wait(lock, [] { return !eventQueue.empty() || stopRequested; });

        while (!eventQueue.empty()) {
            Event event = eventQueue.front();
            eventQueue.pop();
            lock.unlock();

            auto now = std::chrono::steady_clock::now();
            auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(now - event.timestamp).count();
            std::cout << "Обработано: ID=" << event.id
                      << " Msg=" << event.message
                      << " Latency=" << latency << "ms\n";

            // ⏳ ВОТ ЗДЕСЬ — эмулируем «медленного потребителя»
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            lock.lock();
        }
    }
}


void run() {
    stopRequested = false;

    std::cout << "[INFO] Запуск продюсера...\n";
    std::thread prod(producer, 300);

    // ⏳ Даем продюсеру накопить события
    std::this_thread::sleep_for(std::chrono::milliseconds(3500));

    std::cout << "[INFO] Запуск консьюмера...\n";
    std::thread cons(consumer);

    std::cout << "\nМенеджер событий запущен. Нажмите Enter для остановки...\n";
    std::cin.get();

    // 📍 Завершаем работу
    stopRequested = true;
    queueCondVar.notify_all();

    prod.join();
    cons.join();

    std::cout << "Менеджер событий завершён.\n";
}


}
