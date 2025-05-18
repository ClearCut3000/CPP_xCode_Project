#ifndef RPN_CALCULATOR_HPP
#define RPN_CALCULATOR_HPP

#include <string>

namespace RPN {

class Stack {
public:
  Stack();
  bool push(double value);
  bool pop(double& value);
  bool isEmpty() const;
  void clear();
  
private:
  static const int MAX_SIZE = 100;
  double data[MAX_SIZE];
  int topIndex;
};

class RPNCalculator {
public:
  void run();
  
private:
  void evaluate(const std::string& expression);
  bool isNumber(const std::string& s);
  bool isOperator(const std::string& s);
};

} // namespace RPN

#endif // RPN_CALCULATOR_HPP
