#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <chrono>

bool Exit = false;

int main() {
    pid_t mainpid = getpid();
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(22222);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    bind(server_fd, (sockaddr *)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);
    std::cout << "Server on: 127.0.0.1:22222" << std::endl;
    char buf[1024];
    int client_fd;
    if(getpid()==mainpid) {
        while (1) {
            if (getpid() == mainpid) {
                /*    if (Exit) {
                        break;
                        std::cout << "EXIT!" << std::endl;
                    }*/
                auto start = std::chrono::system_clock::now();
                client_fd = accept(server_fd, NULL, NULL);
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> seconds = end - start;
                //std::cout << "Accepted connection: " << client_fd << std::endl;
                if (client_fd < 0) {
                    perror("accept");
                    exit(3);
                }
                if(getpid()==mainpid) {
                    if (fork() == 0) {
                        //std::cout << "Forked! Pid: " << getpid() << std::endl;
                        while (1) {
                            int bytes_read = recv(client_fd, buf, 1024, 0);
                            //std::cout << "Successful received from: " << client_fd << " : " << buf << " pid: " << getpid()
                            //<< std::endl;
                            if (bytes_read <= 0) break;
                            send(client_fd, buf, bytes_read, 0);
                            //std::cout << "Successful sended to: " << client_fd << " pid: " << getpid() << std::endl;
                        }
                        close(client_fd);
                        //std::cout << "Connection " << client_fd << " closed. Process " << getpid() << " finished." << std::endl;
                        return 0;
                    }
                }
                //std::cout << seconds.count() << std::endl;
                if (seconds.count() > 9)
                    break;
            }
        }
    }
    //std::cout << "I am out!!!";

    if (getpid() == mainpid) {
        //int counter = 0;
        while (true) {
            int status;
            pid_t done = wait(&status);
            //counter++;
            //std::cout << counter << std::endl;
            if (done == -1) {
                if (errno == ECHILD) break; // no more child processes
            } else {
                if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                    std::cerr << "pid " << done << " failed" << std::endl;
                    exit(1);
                }
            }
        }
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
    }
    return 0;
}

