//
// Created by moremems on 22/01/2020.
//

#ifndef INC_12_CLIENT_BLOCKED_UNBLOCK_SERVER_H
#define INC_12_CLIENT_BLOCKED_UNBLOCK_SERVER_H
#include<sys/socket.h>
#include <sys/epoll.h>
#include<netinet/in.h>
#include<unistd.h>
#include <vector>
#include <iostream>
#include <error.h>

int start_unblock_server()
{
    char buff[1024];
    // Initialize socket;
    int fd_server = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(22222);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // Bind socket address;
    bind(fd_server, (sockaddr* )&addr, sizeof(addr));
    std::cout << "Server up on: " << "127.0.0.1" << ":" << "22222" << std::endl;
    // Turn on listener;
    listen(fd_server, SOMAXCONN);
    // epoll
    int epfd = epoll_create1(0);
    epoll_event event{EPOLLIN, .data = {.fd = fd_server}};
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd_server, &event);
    std::vector<epoll_event> events(100);
    bool exit = false;
    while (1) {
        if (exit)
            break;
        int nevents = epoll_wait(epfd, events.data(), events.size(), -1);
        //std::cout << "WORKED: " << nevents << std::endl;
        if (nevents == -1) {  // если ошибка
            error(0, errno, "epoll_wait()");
            close(epfd);
            close(fd_server);
            return 1;
        }
        for (int i = 0; i < nevents; ++i) {
            int fd_temp;
            const epoll_event &e = events[i];
            fd_temp = e.data.fd;
            //std::cout << fd_temp << " " << fd_server << std::endl;
            if (fd_temp == fd_server) {
                int client_fd = accept4(fd_server, NULL, NULL, SOCK_NONBLOCK);
                std::cout << "Connected: " << client_fd << std::endl;
                epoll_event event{EPOLLIN, .data = {.fd = client_fd}};
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event)) {
                    error(0, errno, "epoll_ctr(ADD)");
                    close(epfd);
                    close(fd_server);
                    return 1;
                }
            } else {
                char exit_m[] = "exit";
                int bytes = recv(fd_temp, buff, sizeof(buff), 0);
                //std::cout << buff << " " << sizeof(buff) << std::endl;
                if (bytes == -1) {
                    error(0, errno, "recv()");
                    close(epfd);
                    close(fd_server);
                    return 1;
                }
                if (buff[0] == '9'){
                    exit = true;
                    continue;
                }
                if (bytes == 0) {
                    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd_temp, NULL)) {
                        error(0, errno, "epoll_ctr(DEL)");
                        close(epfd);
                        close(fd_server);
                        return 1;
                    }
                    close(fd_temp);
                    std::cout << "Disconnected: " << fd_temp << std::endl;
                    continue;
                }
                if (send(fd_temp, buff, bytes, 0) == -1) {
                    error(0, errno, "send()");
                    close(epfd);
                    close(fd_server);
                    return 1;
                }
            }
        }
    }
    std::cout << "Finishing work" << std::endl;
    close(epfd);
    close(fd_server);
    return 0;
}

#endif //INC_12_CLIENT_BLOCKED_UNBLOCK_SERVER_H
