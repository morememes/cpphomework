#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
#include "victor.h"



int main() {

    victor<std::ofstream> vStreams;
    //std::cout << vStreams.size();
    for (int i = 0; i < 10; i++) {
        std::string name = std::to_string(i) + ".txt";
        vStreams.push_back(std::ofstream(name));
    }
    std::shuffle(vStreams.begin(), vStreams.end(), std::random_device());
    int num = 0;
    for (auto &stream : vStreams) {
        std::cout << num << " ";
        stream << num;
        stream.close();
        ++num;
    }

    /*int a[4] = {1,2,3,4};
    victor<int> v1(a, 4), w;
    victor<int> v;
    v1 = std::move(v1);
    v = std::move(v1);
    v.push_back(4);
    for (auto i = v.begin(); i != v.end(); i++)
        std::cout << *i << " ";*/
    return 0;
}