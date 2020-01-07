#include <iostream>
#include <array>

template  <class Head>
Head& cat(Head& head){
    return head;
}

template <class T, size_t N, class ... Tile>
auto cat(const std::array<T,N>& head, Tile ... tile) -> std::array<T, N * (sizeof...(tile) +1)>{
    std::array<T, sizeof...(tile) * N> tmp(cat(tile...));
    std::array<T, head.size() + tmp.size()> sumArr;
    for (auto i = 0; i < head.size(); i++)
        sumArr[i] = head[i];
    for (auto i = 0; i < tmp.size(); i++)
        sumArr[i + head.size()] = tmp[i];
    return sumArr;
}
