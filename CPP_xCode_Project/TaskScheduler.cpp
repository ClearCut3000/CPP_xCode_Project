#include "TaskScheduler.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <atomic>
#include <mutex>
#include <ctime>

namespace TaskScheduler {

struct Task {
  int priority;
  int delay_ms;
  std::string name;
  std::string type;
  std::chrono::steady_clock::time_point scheduled_time;
  bool recurring = false;               // будет ли повторяться
  int recurring_interval_ms = 0;        // интервал повторения
  std::vector<std::string> depends_on;  // список имён задач, от которых зависит эта задача
  bool executed = false;                // был ли уже выполнен
  std::chrono::system_clock::time_point execution_time; // Время выполнения задачи

  bool operator<(const Task& other) const {
    return priority < other.priority;
  }
};

std::mutex task_mutex;
std::priority_queue<Task> tasks;
std::atomic<bool> stop_flag{false};
std::ofstream log_file("task_log.txt");
std::unordered_map<std::string, Task> task_map;
std::vector<Task> executed_tasks;

void scheduler_thread() {
  while (!stop_flag) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Task task;
    {
      std::lock_guard<std::mutex> lock(task_mutex);
      if (tasks.empty()) continue;

      Task top = tasks.top();

      auto now = std::chrono::steady_clock::now();
      if (top.scheduled_time > now) continue;

      // Проверка зависимостей
      std::ostringstream debug_msg;
      debug_msg << "Проверка задачи: " << top.name << ", зависимость: ";
      bool ready = true;

      for (const auto& dep : top.depends_on) {
        debug_msg << dep << " ";

        // Проверяем, была ли зависимость уже выполнена
        if (task_map.count(dep) && !task_map[dep].executed) {
          ready = false;
        }
      }
      std::cout << debug_msg.str() << std::endl;

      // Если задача не готова, пропускаем её
      if (!ready) continue;

      task = top;
      tasks.pop();
    }

    // Выполнение задачи
    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time_str = std::ctime(&now_time);
    time_str.pop_back(); // убрать \n

    // Устанавливаем время выполнения
    task.execution_time = std::chrono::system_clock::now();

    std::ostringstream exec_msg;
    exec_msg << "[EXECUTED] " << task.name << " (" << task.type << ")"
    << " | Priority: " << task.priority
    << ", Delay: " << task.delay_ms << "ms"
    << ", Time: " << time_str;

    if (!task.depends_on.empty()) {
      exec_msg << ", after dependencies: ";
      for (size_t i = 0; i < task.depends_on.size(); ++i) {
        exec_msg << task.depends_on[i];
        if (i + 1 < task.depends_on.size()) exec_msg << ", ";
      }
    }

    std::string exec_output = exec_msg.str();
    log_file << exec_output << "\n";
    std::cout << exec_output << "\n";

    // Добавляем выполненную задачу в список
    executed_tasks.push_back(task);

    // Обновление состояния зависимостей
    for (const auto& dep : task.depends_on) {
      task_map[dep].executed = true;  // Обновляем зависимость как выполненную
    }

    // Перезапуск
    if (task.recurring) {
      task.scheduled_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(task.recurring_interval_ms);
      task.executed = false;
      std::lock_guard<std::mutex> lock(task_mutex);
      tasks.push(task);
    }
  }
}

void run() {
  std::cout << "\n== Планировщик задач ==\n";
  std::cout << "Введите задачи в формате:\n"
  << "priority delay(ms) name type [recurring_interval] [depends_on1,depends_on2,...]\n";
  std::cout << "Пример: 10 1000 Backup DB\n";
  std::cout << "Введите 'start' чтобы запустить, 'exit' для отмены.\n";

  std::string command;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command == "start") break;
    if (command == "exit") return;

    std::istringstream iss(command);
    int prio, delay;
    std::string name, type;

    if (iss >> prio >> delay >> name >> type) {
      Task task;
      task.priority = prio;
      task.delay_ms = delay;
      task.name = name;
      task.type = type;
      task.scheduled_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(delay);

      // Чтение recurring
      int recur = 0;
      if (iss >> recur && recur > 0) {
        task.recurring = true;
        task.recurring_interval_ms = recur;
      }

      // Чтение зависимостей
      std::string deps_str;
      if (iss >> deps_str) {
        std::istringstream deps_stream(deps_str);
        std::string dep;
        while (std::getline(deps_stream, dep, ',')) {
          task.depends_on.push_back(dep);
        }
      }

      {
        std::lock_guard<std::mutex> lock(task_mutex);
        tasks.push(task);
        task_map[task.name] = task;
      }

      std::cout << "Добавлено: " << name << " (" << type << ") c приоритетом " << prio << "\n";
    }

    std::cout << "[DEBUG] Всего задач в очереди: " << tasks.size() << "\n";
  }

  stop_flag = false;
  std::cout << "[INFO] Планировщик запущен...\n";
  std::thread scheduler(scheduler_thread);

  std::cout << "Нажмите Enter для остановки планировщика...\n";
  std::cin.get();
  stop_flag = true;

  scheduler.join();
  std::cout << "Планировщик завершён.\n";

  // Вывод о выполненных задачах
  std::cout << "\n== Выполненные задачи ==\n";
  for (const auto& task : executed_tasks) {
    time_t execution_time = std::chrono::system_clock::to_time_t(task.execution_time);
    std::cout << "[EXECUTED] " << task.name << " | Тип: " << task.type
    << " | Время: " << std::ctime(&execution_time)
    << " Зависимости: ";
    for (const auto& dep : task.depends_on) {
      std::cout << dep << " ";
    }
    std::cout << "\n";
  }

  log_file.close();
}
}
