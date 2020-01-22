#include <vector>
#include "utils.h"
#include "client_blocked.h"
#include "unblock_server.h"

std::vector<data_test> results;
int main(int argc, char* argv[]) {
    if (argc == 6) {
        pid_t main_pid = getpid();
        char *message = argv[1];
        int clients_number = atoi(argv[2]);
        int iters = atoi(argv[3]);
        int shift = atoi(argv[4]);
	bool block = atoi(argv[5]);
        std::cout << "Params: " << message << " " << clients_number << " " << iters << " " << shift << " " << block << std::endl;

        /*if (getpid() == main_pid) {
            if (fork() == 0) {
                std::cout << "Starting unblock server..." << std::endl;
                start_unblock_server();
            }
        }*/
        server_test(results, clients_number, iters, shift, message, main_pid);
        if (getpid() == main_pid) {
            if (block == 1) {
                std::cout << "Waiting 11 sec, server is shutting down.\n";
                sleep(11);
            }
            shutdown_server();
            //int status;
            //wait(&status);
        }

        if (getpid() == main_pid) {
            for (auto &el : results) {
                std::cout << "Clients number: " << el.clients << ". Time passed: " << el.timer
                          << ". Speed of connection: "
                          << el.speed << " iter/s." << std::endl;
            }
        }
    }
    else{
        std::cout << "Enter 4 parametrs. Message(char*), client_max_number(int), iters(int), shift(int), block(0/1)";
    }
    return 0;
}

