#include <math.h>
#include <iostream>
#include <vector>
#include "threadPool.h"

double TestFunction(double a, double b, double c)
{
  std::cout << "executing func" << std::endl;
  return a;  // std::pow(a, 3) * std::pow(b, 2) / std::pow(c, 2);
}

int main()
{
  ThreadPool tp;
  const unsigned long complexity = 100;
  std::function<double(void)> func;
  std::vector<std::future<double>> results{complexity};
  for (unsigned long i = 0; i < complexity; i++)
    {
      func = [i]() { return TestFunction(i, 2.0, 1.0); };
      results.emplace_back(tp.Do(func));
    }

  for (unsigned long i = 0; i < complexity; i++)
    {
      results[i].wait();
      std::cout << results[i].get() << std::endl;
    }

  return 0;
}
