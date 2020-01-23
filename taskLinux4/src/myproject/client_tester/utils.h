//
// Created by moremems on 22/01/2020.
//

#ifndef INC_12_CLIENT_BLOCKED_UTILS_H
#define INC_12_CLIENT_BLOCKED_UTILS_H

#include <vector>
#include <chrono>
#include <unistd.h>
#include "client_blocked.h"

struct data_test{
    int clients;
    double timer;
    double speed;
    bool correct;
};

void server_test(std::vector<data_test> &results, int clients_number, int iters, int shift, char message[], pid_t main_pid) {
    //std::cout << "Starting test of server..." << std::endl;
    if (getpid() == main_pid) {
        for (int clients = 1; clients < clients_number + 1; clients = clients + shift - 1 * (clients == 1)) {
            if (getpid() == main_pid) {
                bool correct;
                auto start = std::chrono::system_clock::now();
                test_server(message, clients, main_pid, correct, iters);
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> seconds = end - start;
                results.push_back({clients, seconds.count(), double(iters * clients) / seconds.count(), correct});
            }
        }
    }
}

#endif //INC_12_CLIENT_BLOCKED_UTILS_H

