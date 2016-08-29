/* C++11 new features testing: "std::condition_variable and std::mutex"
This post shows combination of several new C++11 features:

    std::condition_variable
    std::mutex
    std::thread
    std::ref - passing parameters into thread by reference
    lambda - anonymous functions

and implements semaphore using std::condition_variable and std::mutex .
*/


 #include<mutex>  
 #include<condition_variable>  
 #include<thread>  
 #include<chrono>  
 #include<random>  
 #include<iostream>  


 typedef std::ranlux48 TRandomGenerator;  
 class CSemaphore  
 {  
 private:  
   std::mutex mutex_;  
   std::condition_variable condition_;  
   unsigned int count_;  
 public:  
   CSemaphore()  
     : count_()  
   {}  
   void reset()  
   {  
        std::unique_lock<std::mutex> lock(mutex_);  
     count_=0;  
   }  
   void signal()  
   {  
        std::unique_lock<std::mutex> lock(mutex_);  
     ++count_;  
     condition_.notify_one();  
   }  
   void wait()  
   {  
        std::unique_lock<std::mutex> lock(mutex_);  
     while(!count_)  
       condition_.wait(lock);  
     --count_;  
   }  
 };  


 int main()  
 {  
      int valueToCompute=0;  
      CSemaphore sem;  
      std::thread(  
           [](int &valueToCompute,CSemaphore&sem)  
           {  
                TRandomGenerator randomGenerator( time(0) );  
                std::uniform_int_distribution<uint32_t> rg1000(0,1000);   
                uint sleepDuration=rg1000(randomGenerator);  
                std::cout<<"producer:"<<"("<<sleepDuration<<")"<<std::endl;  
                std::this_thread::sleep_for(   
                          std::chrono::milliseconds(sleepDuration) );  
                valueToCompute=333;  
                sem.signal();  
           },  
           std::ref(valueToCompute),  
           std::ref(sem)  
      ).detach();  
      sem.wait();  
      std::cout<<"computed value:"<<valueToCompute<<std::endl;  
      return 0;  
 }  


