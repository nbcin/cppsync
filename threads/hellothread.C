#include <iostream>
#include <thread>

using namespace std;

void hello_world()
{
  cout << "Hello from thread!\n";
}

int main()
{
  thread t(hello_world);
  t.join();
  return 0;
}
