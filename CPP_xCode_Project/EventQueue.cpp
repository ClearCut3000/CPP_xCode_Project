#include "EventQueue.hpp"  // Подключаем интерфейс

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <string>
#include <vector>
#include <numeric>

// Структура события
struct Event {
    int id;
    std::string message;
    std::chrono::steady_clock::time_point created_at;
};

// Потокобезопасная очередь событий
class EventQueue {
private:
    std::queue<Event> queue;
    std::mutex mutex;
    std::condition_variable cv;

public:
    void push(const Event& event) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(event);
        }
        cv.notify_one();
    }

    bool pop(Event& event, std::atomic<bool>& stop_requested) {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&]() { return !queue.empty() || stop_requested.load(); });

        if (!queue.empty()) {
            event = queue.front();
            queue.pop();
            return true;
        }
        return false;
    }
};

// Основная логика, вызываемая из main
void run_event_manager() {
    EventQueue eventQueue;
    std::atomic<bool> stop_requested(false);
    std::vector<long long> latencies_ms;

    std::thread producer([&]() {
        int counter = 0;
        while (!stop_requested.load()) {
            Event event;
            event.id = counter++;
            event.message = "Event #" + std::to_string(event.id);
            event.created_at = std::chrono::steady_clock::now();

            eventQueue.push(event);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    std::thread consumer([&]() {
        while (!stop_requested.load()) {
            Event event;
            if (eventQueue.pop(event, stop_requested)) {
                auto now = std::chrono::steady_clock::now();
                auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(now - event.created_at).count();
                latencies_ms.push_back(latency);

                std::cout << "[Consumer] Processed: " << event.message
                          << " | Latency: " << latency << "ms\n";

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    stop_requested.store(true);

    producer.join();
    consumer.join();

    if (!latencies_ms.empty()) {
        long long total = std::accumulate(latencies_ms.begin(), latencies_ms.end(), 0LL);
        double avg_latency = static_cast<double>(total) / latencies_ms.size();
        std::cout << "\nAverage latency: " << avg_latency << " ms over "
                  << latencies_ms.size() << " events.\n";
    } else {
        std::cout << "\nNo events processed.\n";
    }
}
