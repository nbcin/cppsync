#include <iostream>
#include <thread>
#include <queue>

using namespace std;

class data_chunk {
public:
  bool isLast;
  long id;
};
 
mutex mutex1;
mutex mutex2;

// Used to send data between threads
 queue<data_chunk> data_queue;

 condition_variable data_cond;

 const int FIRST = 5;
 const int LAST = 0;

 int chunks = FIRST;

 data_chunk prepare_data()
 {  
   lock_guard<mutex> lk(mutex2);
     data_chunk* e = new data_chunk();
       e->isLast = (LAST == chunks);
         e->id = chunks--;
           return *e;
           }

           bool has_data()
           {
             lock_guard<mutex> lk(mutex2);
               return chunks >= LAST;
               }

               void process(data_chunk data)
               {
                 cout << "PROCESS: Processing chunk " << data.id << endl;
                 }

                 bool is_last_chunk(data_chunk data)
                 {
                   return data.isLast;
                   }

                   void prepare_data_thread()
                   {
                     while(has_data())
                         {
                               data_chunk const data = prepare_data();
 			cout << "PREPARE: I give you: " << data.id << endl;
                       lock_guard<mutex> lk(mutex1);
                       data_queue.push(data);

                       // Notify waiting thread
                       data_cond.notify_one();
 			}
     cout << "PREPARE: I give you no more data!" << endl;
     }

     void process_data_thread()
                               {
               while(true)
               {
               			unique_lock<mutex> lk(mutex1);
               			cout << "PROCESS: I will wait for stuff ..." << endl;

 // Wait(mutex, waiting condition)
 // If the wait fails, the mutex is unlocked and
 // the threads enters in a waiting state;
 // when data_cond is notified through a notify_one(),
 // the thread resumes, reacquiring the lock on the mutex,
 // and checks the condition again
       data_cond.wait(lk, []{ return !data_queue.empty(); });
       cout << "PROCESS: I have data to process!" << endl;
       data_chunk data = data_queue.front();
       data_queue.pop();
       lk.unlock();
       process(data);
       
		if(is_last_chunk(data))
               {
               	cout << "PROCESS: I'm done!" << endl;
               	break;
               }
                                     
     }
    	cout << "PROCESS: No more processing!" << endl;
    }


int main(int argc, char** argv)
{
thread process(process_data_thread);
thread prepare(prepare_data_thread);
prepare.join();
process.join();
return 0;
}

