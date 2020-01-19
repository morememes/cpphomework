#include <iostream>
#include <array>
#include <random>
#include <thread>
#include <pthread.h>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <unistd.h>
#include "../MyThread.h"


#define THREAD_MAX std::thread::hardware_concurrency()
std::vector<MyThread> vecThr{THREAD_MAX};

template<class T, size_t N>
std::array<T, N> generateArr(){
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::array<T, N> arr;
    for (auto &el : arr)
        el = mersenne() % 100;
    return arr;
}

template <class Iter>
void MergeSort(Iter b, Iter e);

template <class Iter>
void BackSort(Iter s1, Iter e1, Iter s2, Iter e2);

void test(){
    std::cout << "test";
}

template <class Iter>
int my_sort(Iter a, Iter b){
    std::sort(a, b);
    std::cout << "My sort has worked." << std::endl;
    return 0;
}
template <class Iter>
void thread_runner(Iter a, Iter b){
    MyThread thr([a, b](){ return wrapper(std::sort<Iter>, a, b);});
    thr.join();
}

template <class Iter>
void MergeSortM(Iter b, Iter e){
    //unsigned int n = std::thread::hardware_concurrency();
    //std::cout << n << " concurrent threads are supported.\n";
    size_t size_arr = e - b;
    size_t shift = size_arr / THREAD_MAX;

    for (int i = 0; i < THREAD_MAX-1; i++)
    {
        thread_runner(b + shift*i, b + shift*(i+1));
    }
    thread_runner(b + 3*shift, e);
    BackSort(b, b+shift, b+shift, b+shift*2);
    BackSort(b + shift*2, b+shift*3, b+shift*3, e);
    BackSort(b, b+shift*2, b+shift*2 , e);
}

template <size_t N>
void testSort(){
    std::array<int, N> arr = generateArr<int, N>();
    std::array<int, N> arr1(arr);
    //for (auto &el : arr)
    //   std::cout << el << " ";
    //std::cout << std::endl;
    auto chp1 = std::chrono::system_clock::now();
    MergeSort(arr1.begin(), arr1.end());
    auto chp2 = std::chrono::system_clock::now();
    MergeSortM(arr.begin(), arr.end());
    auto chp3 = std::chrono::system_clock::now();
    //for (auto &el : arr)
    //    std::cout << el << " ";
    //std::cout << chp1 << " "  << chp2.count() << " " << chp3.count() << " " << std::endl;
    std::cout << "Size: " << N << " Result: " << (arr == arr1 ? "OK" : "FAIL") << " Threads: " << (chp3 - chp2).count() << " WithoutThreads: " << (chp2 - chp1).count() << std::endl;
}

template <class T>
struct mytraits{
    typedef typename T::value_type type;
};

template <class T>
struct mytraits<T*>{
    typedef typename std::remove_pointer<T>::type type;
};

template <class Iter>
void MergeSort(Iter b, Iter e) {
    typedef typename mytraits<Iter>::type type;
    if ((e - b) > 2) {
        MergeSort(b, b + (e - b) / 2);
        MergeSort(b + (e - b) / 2, e);
    }
    Iter sep = b + (e - b) / 2;
    Iter b1 = b + (e - b) / 2;
    Iter bc = b;
    const size_t size_arr = e - b;
    type *a = new type[size_arr];
    int i = 0;
    while (b < sep && b1 < e) {
        if (*b > *b1)
            a[i] = *b1++;
        else
            a[i] = *b++;
        i++;
    }
    while (b1 < e)
        a[i++] = *b1++;
    while (b < sep)
        a[i++] = *b++;
    i = 0;
    while (bc < e)
        *bc++ = a[i++];
}

template <class Iter>
void BackSort(Iter s1, Iter e1, Iter s2, Iter e2) {
    //std::cout << "test BackSort";
    typedef typename mytraits<Iter>::type type;
    Iter begin = s1;
    const size_t size_arr = e2 - s1;
    type *a = new type[size_arr];
    int i = 0;
    while(s1 < e1 && s2 < e2){
        if (*s1 > *s2)
            a[i] = *s2++;
        else
            a[i] = *s1++;
        i++;
    }
    while(s1 < e1)
        a[i++] = *s1++;
    while(s2 < e2)
        a[i++] = *s2++;
    i = 0;
    while(begin < e2)
        *begin++ = a[i++];

    delete [] a;
}

