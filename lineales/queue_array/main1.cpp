#include <iostream>
#include "queue_array.h"



int main() {
  QueueArray<int> q;
  q.push(10);
  q.push(20);
  q.push(30);

  std::cout << q.front() << std::endl;
  q.pop();
  std::cout << q.front() << std::endl;
  q.pop();
  std::cout << q.front() << std::endl;
  q.pop();

  
}
