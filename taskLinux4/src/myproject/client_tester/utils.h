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
};

void server_test(std::vector<data_test> &results, int clients_number, int iters, int shift, char message[],  pid_t main_pid) {
    std::cout << "Starting test of server..." << std::endl;
    for (int clients = 1; clients < clients_number + 1; clients = clients + shift - 1*(clients == 1)) {
        if (getpid() == main_pid) {
            auto start = std::chrono::system_clock::now();
            test_server(message, clients, main_pid, iters);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> seconds = end - start;
            results.push_back({clients, seconds.count(), double(iters * clients) / seconds.count()});
        }
    }
}

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
#endif //INC_12_CLIENT_BLOCKED_UTILS_H
