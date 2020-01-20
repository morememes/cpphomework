#include <iostream>

#include <vector>
#include <algorithm>
#include "MyThread.h"
#include "8task/task8.h"

int main() {
    
    testSort<20>();
    testSort<50>();
    testSort<100>();
    testSort<500>();
    testSort<1000>();
    testSort<2000>();
    testSort<4000>();
    testSort<8000>();
    testSort<12000>();
    testSort<16000>();
    testSort<20000>();

    return 0;
}
