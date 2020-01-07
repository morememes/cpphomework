#include <iostream>
#include <tuple>
#include <vector>

template <class T, size_t N, size_t M>
struct MyTie {
    std::array<T*, M> ArrOfArr;
    size_t size;
    MyTie(std::array<T*, M> input) : ArrOfArr(input) {}
    void operator=(const std::array<T,N*M>& rhs) {
        for (int i = 0; i < M; i++)
            for (int k = 0; k < N; k++)
                *(ArrOfArr[i] + k) = rhs[N * i + k];
    }
};

template <class T, size_t N, class ... Tile>
auto MyTieF(std::array<T, N>& head, Tile& ... tile) -> MyTie<T, N, sizeof...(Tile) + 1> {
    std::array<T*, sizeof...(Tile) + 1> resArr{head.data(), tile.data()...};
    return MyTie<T, N, sizeof...(Tile) + 1>(resArr);
}
