//
// Created by moremems on 18/01/2020.
//

#ifndef INC_11_MYTHREAD_H
#define INC_11_MYTHREAD_H

#include <functional>
#include <iostream>
// Wrapper for functions
template <class Fun, class ... Args>
int wrapper(Fun *fun, Args ... args){
    (*fun)(args...);
    return 0;
}


// CLASS MYTHREAD
class MyThread{
private:
    int pid = 0;
    char* stackBot;
    char* stackTop;
public:
    MyThread() = default;
    MyThread(std::function<int()> &&f);
    ~MyThread();
    // Methods
    bool joinable();
    int join();
    int get_id();
    void swap(MyThread &thr);
    // Operators
    MyThread& operator=(MyThread &&thr);
};


#endif //INC_11_MYTHREAD_H
