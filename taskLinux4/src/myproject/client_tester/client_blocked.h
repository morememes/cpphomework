//
// Created by moremems on 22/01/2020.
//

#ifndef INC_12_CLIENT_BLOCKED_CLIENT_BLOCKED_H
#define INC_12_CLIENT_BLOCKED_CLIENT_BLOCKED_H

#include <iostream>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define CHECK(func) \
if ((func) == -1) { \
throw std::system_error(errno, std::generic_category());\
}

void shutdown_server(){
    int bufsize = 1024;
    char message[] = "9";
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(22222);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockaddr_in any{};
    bind(fd, (sockaddr* ) &any, sizeof(any));
    connect(fd, (sockaddr*) &addr, sizeof(addr));
    send(fd, message, sizeof(message), 0);
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void client_start(char message[], int iters = 15, int bufsize = 1024) {
    //std::cout << message << std::endl;
    char buf[bufsize];

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(22222);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockaddr_in any{};
    bind(fd, (sockaddr* ) &any, sizeof(any));
    connect(fd, (sockaddr*) &addr, sizeof(addr));

    for (int i=0; i < iters; i++) {
        send(fd, message, bufsize, 0);
        recv(fd, buf, bufsize, 0);
        //std::cout << buf << std::endl;
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void test_server(char message[], int clients_number, pid_t main_pid, int iters = 15){
    for (int i=0;i<clients_number;i++){
        if (getpid() == main_pid) {
            if (fork() == 0) {
                //std::cout << "Process created: " << getpid() << std::endl;
                client_start(message, iters = iters);
            }
        }
    }
    if (getpid() == main_pid) {
        while (true) {
            int status;
            pid_t done = wait(&status);
            if (done == -1) {
                if (errno == ECHILD) break; 
            } else {
                if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                    std::cerr << "pid " << done << " failed" << std::endl;
                    exit(1);
                }
            }
        }
        //if (getpid() == main_pid)
        //    std::cout << "Finishing test_server...\n";
    }
}

#endif //INC_12_CLIENT_BLOCKED_CLIENT_BLOCKED_H

