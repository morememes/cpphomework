//
// Created by moremems on 18/01/2020.
//

#include "MyThread.h"
//Includes For Threads
#include <sched.h>
#include <system_error>
#include <functional>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

//Some Utils

#define CHECK(fun) \
    if((fun) == -1) { \
        throw std::system_error(errno, std::generic_category()); \
    }

int funrunner(void *arg){
    auto *f = static_cast<std::function<int()>*>(arg);
    return (*f)();
}


// Class MyThreads
#define SIZEOFSTACK 1024*1024

MyThread::MyThread(std::function<int()> &&f){
    stackBot = new char[SIZEOFSTACK];
    stackTop = stackBot + SIZEOFSTACK -1;
    pid = clone(funrunner, stackTop, CLONE_VM | SIGCHLD, &f);
    CHECK(pid);
}
MyThread::~MyThread(){
    if (pid != 0){
        kill(pid, SIGTERM);
        delete[] stackBot;
    }
}

bool MyThread::joinable(){
    return kill(pid, 0) == 0;
}
int MyThread::join(){
    int status;
    CHECK(waitpid(pid, &status, 0));
    return status;
}
int MyThread::get_id(){
    return pid;
}
void MyThread::swap(MyThread &thr){
    std::swap(this->pid, thr.pid);
    std::swap(this->stackBot, thr.stackBot);
    std::swap(this->stackTop, thr.stackTop);
}

MyThread& MyThread::operator=(MyThread &&thr){
    this->swap(thr);
    return *this;
}
