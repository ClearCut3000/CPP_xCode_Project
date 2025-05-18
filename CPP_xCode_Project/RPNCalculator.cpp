#include "RPNCalculator.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

/*
 Примеры ввода:
 === Обратная польская нотация (ОПН) ===
 Введите выражение в ОПН или 'exit' для выхода.

 > 3 4 + 2 * 7 /
 Результат: 2

 > 3 +
 Ошибка: недостаточно операндов для операции '+'.

 >  4 0 /
 Ошибка: деление на ноль.

 > 2 2 foo
 Ошибка: недопустимый токен 'foo'.
 */

namespace RPN {

// ------------------- Stack -------------------

Stack::Stack() : topIndex(-1) {}

bool Stack::push(double value) {
  if (topIndex >= MAX_SIZE - 1) return false;
  data[++topIndex] = value;
  return true;
}

bool Stack::pop(double& value) {
  if (topIndex < 0) return false;
  value = data[topIndex--];
  return true;
}

bool Stack::isEmpty() const {
  return topIndex == -1;
}

void Stack::clear() {
  topIndex = -1;
}

// ------------------- RPNCalculator -------------------

void RPNCalculator::run() {
  std::cout << "=== Обратная польская нотация (ОПН) ===\n";
  std::cout << "Введите выражение в ОПН или 'exit' для выхода.\n";

  std::string line;
  while (true) {
    std::cout << "\n> ";
    std::getline(std::cin, line);

    if (line == "exit") {
      std::cout << "Завершение работы калькулятора.\n";
      break;
    }

    evaluate(line);
  }
}

void RPNCalculator::evaluate(const std::string& expression) {
  std::istringstream iss(expression);
  std::string token;
  Stack stack;
  double a, b;

  while (iss >> token) {
    if (isNumber(token)) {
      stack.push(std::stod(token));
    } else if (isOperator(token)) {
      if (!stack.pop(b) || !stack.pop(a)) {
        std::cout << "Ошибка: недостаточно операндов для операции '" << token << "'.\n";
        return;
      }

      if (token == "+") {
        stack.push(a + b);
      } else if (token == "-") {
        stack.push(a - b);
      } else if (token == "*") {
        stack.push(a * b);
      } else if (token == "/") {
        if (b == 0) {
          std::cout << "Ошибка: деление на ноль.\n";
          return;
        }
        stack.push(a / b);
      }
    } else {
      std::cout << "Ошибка: недопустимый токен '" << token << "'.\n";
      return;
    }
  }

  double result;
  if (!stack.pop(result) || !stack.isEmpty()) {
    std::cout << "Ошибка: некорректное выражение (возможно, лишние операнды).\n";
  } else {
    std::cout << "Результат: " << result << "\n";
  }
}

bool RPNCalculator::isNumber(const std::string& s) {
  char* end = nullptr;
  std::strtod(s.c_str(), &end);
  return end != s.c_str() && *end == '\0';
}

bool RPNCalculator::isOperator(const std::string& s) {
  return s == "+" || s == "-" || s == "*" || s == "/";
}

} // namespace RPN

