#include <iostream>
#include <array>
#include <random>
#include <thread>
#include <pthread.h>
#include <iterator>
#include <algorithm>
#include <type_traits>

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
void MergeSortM(Iter b, Iter e){
    unsigned int n = std::thread::hardware_concurrency();
    //std::cout << n << " concurrent threads are supported.\n";
    size_t size_arr = e - b;
    size_t shift = size_arr / n;
    std::vector<std::thread> vecThr;
    for (int i = 0; i < n-1; i++)
    {
        vecThr.push_back(
                std::thread(MergeSort<Iter>, b + shift*i, b + shift*(i+1)) );
    }
    vecThr.push_back(
            std::thread(MergeSort<Iter>, b + (n-1)*shift, e));
    for (auto &thr : vecThr)
        thr.join();
    BackSort(b, b+shift, b+shift, b+shift*2);
    BackSort(b + shift*2, b+shift*3, b+shift*3, e);
    BackSort(b, b+shift*2, b+shift*2 , e);
}

template <size_t N>
void testSort(){
    std::array<int, N> arr =  generateArr<int, N>();
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
    std::cout << "Size: " << N << " Result: " << (arr == arr1) << " Threads: " << (chp3 - chp2).count() << " WithoutThreads: " << (chp2 - chp1).count() << std::endl;
}

int main() {
    const size_t N = 1000;
    const size_t sizes[] = {10, 50, 100, 500, 1000, 2000, 4000};
    std::vector<int> vec = { 9,5,4,3,2,1,6,7,5,10 };
    testSort<10>();
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