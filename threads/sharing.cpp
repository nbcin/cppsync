#include <thread>
#include <iostream>
#include <list>
#include <algorithm>
#include <mutex>

std::list<int> list1;
std::mutex mutex1;

void add_list1(int val)
{
  std::lock_guard<std::mutex> guard(mutex1);
  list1.push_back(val);
}

bool list1_contains(int val)
{
  std::lock_guard<std::mutex> guard(mutex1);
  return std::find(list1.begin(), list1.end(), val) != list1.end();
}

void f()
{
 std::cout<<"Hello from function f()"<<endl;

  }
  
  
  int main(int argc, char** argv)
  {
    std::thread t(f);
       t.join();
         return 0;
         }
