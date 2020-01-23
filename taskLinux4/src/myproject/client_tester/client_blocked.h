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

int sendall(int s, char *buf, int len, int flags){
    int total = 0;
    int n;
    while(total < len){
        n = send(s, buf+total, len-total, flags);
        if(n == -1) { break; }
        total += n;
    }
    return (n==-1 ? -1 : total);
}

int recvall(int s, char *buf, int len, int flags){
    int total = 0;
    int n;
    while (total < len){
        n = recv(s, buf+total, len-total, flags);
        if (n == -1) {break;}
        total +=n;
    }
    return (n==-1 ? -1 : total);
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

void client_start(char message[], bool &correct,  int iters = 15, int bufsize = 1024*1024*2) {
    char buf[bufsize];
    for (int i=0; i < std::strlen(message); i++)
        buf[i] = message[i];
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(22222);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockaddr_in any{};
    bind(fd, (sockaddr* ) &any, sizeof(any));
    connect(fd, (sockaddr*) &addr, sizeof(addr));

    correct = true;
    int bytes;
    int sended;
    for (int i=0; i < iters; i++) {
        //std::cout << buf << std::endl;
        sended = sendall(fd, buf, bufsize, 0);
        if (sended < 0){
            perror("send");
            exit(2);
        }
        //std::cout << sended << " Sended" << std::endl;
        bytes = recvall(fd, buf, bufsize, 0);
        //std::cout << buf << std::endl;
        if (bytes < 0){
            perror("send");
            exit(2);
        }
        if (std::strncmp(message, buf, std::strlen(message)) != 0)
            correct = false;

        //std::cout << (std::strncmp(message, buf, std::strlen(message)) == 0 ? "OK" : "FAIL") << std::endl;
        //std::cout << bytes << std::endl << buf << std::endl;
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void test_server(char message[], int clients_number, pid_t main_pid, bool& correct,int iters = 15){
    for (int i=0;i<clients_number;i++){
        if (getpid() == main_pid) {
            if (fork() == 0) {
                //std::cout << "Process created: " << getpid() << std::endl;
                client_start(message, correct, iters = iters);
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

