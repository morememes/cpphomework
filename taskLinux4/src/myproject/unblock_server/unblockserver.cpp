#include<sys/socket.h>
#include <sys/epoll.h>
#include<netinet/in.h>
#include<unistd.h>
#include <vector>
#include <iostream>
#include <error.h>
#include <cstring>
#include <algorithm>

#define buff_size 1024*1024*2

struct client_data_struct{
    char buff[buff_size];
    bool read_finished = false;
    bool write_finished = false;
    int byte_read = 0;
    int byte_sended = 0;
};

std::vector<client_data_struct> clientsdata;
std::vector<int> clientdata_dfid;

int sendall(int s, char *buf, int len, int flags);
int recvall(int s, char *buf, int len, int flags);
int find_idx(std::vector<int>& vec, int el){
    auto idx = std::find(vec.begin(), vec.end(), el);
    int idx_int = idx - vec.begin();
    return (idx_int == vec.size() ? -1 : idx_int);
}

int main()
{
    //int buff_size = 1024*1024*2;
    char buff[buff_size];
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
        int idc;
        int bytes;
        int bytes_batch;
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
                //std::cout << "Connected: " << client_fd << std::endl;
                epoll_event event{EPOLLIN, .data = {.fd = client_fd}};
                client_data_struct cl;
                clientsdata.push_back(cl);
                clientdata_dfid.push_back(client_fd);
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event)) {
                    error(0, errno, "epoll_ctr(ADD)");
                    close(epfd);
                    close(fd_server);
                    return 1;
                }
            } else {
                idc = find_idx(clientdata_dfid, fd_temp);
                client_data_struct &current_client = clientsdata[idc];
                bytes = recv(fd_temp, current_client.buff + current_client.byte_read, buff_size - current_client.byte_read, 0);
                if (bytes == -1) {
                    error(0, errno, "recv()");
                    close(epfd);
                    close(fd_server);
                    return 1;
                }
                if (!current_client.read_finished) {
                    //std::cout << "readed" << " " << bytes << std::endl;
                    current_client.byte_read += bytes;
                    if (current_client.byte_read == buff_size)
                        current_client.read_finished = true;
                        current_client.write_finished = false;
                        current_client.byte_sended = 0;
                }
                if (current_client.buff[0] == '9'){
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
                    clientsdata.erase(clientsdata.begin() + idc);
                    clientdata_dfid.erase(clientdata_dfid.begin() + idc);
                    //std::cout << "Disconnected: " << fd_temp << std::endl;
                    continue;
                }
                if (current_client.read_finished && !current_client.write_finished) {
                    if (current_client.byte_sended == 0) {
                        epoll_event event{EPOLLOUT, .data = {.fd = fd_temp}};
                        if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd_temp, &event)) {
                            error(0, errno, "epoll_ctr(MOD)");
                            close(epfd);
                            close(fd_server);
                            return 1;
                        }
                    }
                    int sended = send(fd_temp, current_client.buff + current_client.byte_sended, buff_size - current_client.byte_sended, 0);
                    //std::cout << "sended:" << sended << std::endl;
                    if (sended == -1) {
                        error(0, errno, "send()");
                        close(epfd);
                        close(fd_server);
                        return 1;
                    }
                    current_client.byte_sended += sended;
                    if (current_client.byte_sended == buff_size) {
                        current_client.write_finished = true;
                        current_client.read_finished = false;
                        current_client.byte_read = 0;
                        epoll_event event{EPOLLIN, .data = {.fd = fd_temp}};
                        if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd_temp, &event)) {
                            error(0, errno, "epoll_ctr(MOD)");
                            close(epfd);
                            close(fd_server);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    std::cout << "Finishing work..." << std::endl;
    close(epfd);
    close(fd_server);
  return 0;
}

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
        //std::cout << total << std::endl;
    }
    return (n==-1 ? -1 : total);
}
