#include <unistd.h>
#include <thread> 
#include <deque>
#include <iostream>
#include <mutex>
#include <condition_variable>

const int THREADS = 3;
std::thread threads[THREADS];
std::condition_variable condition_variable;
std::deque<int> requests;
std::mutex mutex;

void worker() {
    while (1) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            while (requests.size() == 0)
            {
                condition_variable.wait(lock);
            }
            int num = requests.front();
            requests.pop_front();
            std::cout<<"consumer: "<<num<<", thread: "<<(uint64_t)pthread_self()<<std::endl;
        }
    }
}

int main() 
{ 
   
    for (int i = 0; i < THREADS; i++) {
        threads[i] = std::thread(worker);
    }
    int j = 0;
    while(1) {
        {   
            sleep(1);
            std::lock_guard<std::mutex> lock(mutex);
            requests.push_back(j++);
            condition_variable.notify_all();
        }
    }

    for (int i = 0; i < THREADS; i++) {
        threads[i].join();
    }

    return 0;
} 