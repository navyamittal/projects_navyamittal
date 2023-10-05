#include "functions.h"
using std::cin, std::cout, std::endl, std::ostream, std::string;

#define INFO(X)  cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;
#define INFO_STRUCT(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " count = " << X.count << endl;

/**
 * ----- REQUIRED -----
 * Pushes number to top of stack. If stack is full, then resize stack's array.
 * @param   stack   Target stack.
 * @param   number  Number to push to stack.
 */
void push(Stack& stack, int number) {
  // TODO: implement push function for stack
  INFO_STRUCT(stack);
  INFO(number);
  
  if (stack.count==stack.capacity){
    //stack.capacity *=2;
    int* temp = new int[stack.capacity *=2];
    temp[stack.count]=number;
    for (int i =0; i<stack.count; i++){
      temp[i]=stack.numbers[i];
    }
    stack.count+=1;
    delete stack.numbers;
    stack.numbers= temp;
    temp = nullptr;
  }
  else if (stack.count!=stack.capacity){
    stack.numbers[stack.count]=number;
    stack.count+=1;
  }
  //stack.numbers[stack.count]=number;
}

/**
 * ----- REQUIRED -----
 * Pops number from top of stack. If stack is empty, return INT32_MAX.
 * @param   stack   Target stack.
 * @return          Value of popped number.
 */
int pop(Stack& stack) {
  // TODO: implement pop function for stack
  INFO_STRUCT(stack);
  if (stack.count==0){
    return INT32_MAX;
  }
  stack.count-=1;
  return stack.numbers[stack.count];
  
}

/**
 * ----- OPTIONAL -----
 * Returns the number at top of stack without popping it. If stack is empty, return INT32_MAX.
 * @param   stack   Target statck.
 * @return          Number at top of stack.
 */
int peek(const Stack& stack) {
  // TODO (optional): implement peek function for stack
  INFO_STRUCT(stack);
  return stack.numbers[stack.count-1];
  return 0;
}
