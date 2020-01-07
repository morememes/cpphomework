//
// Created by morememes on 25.10.2019.
//
#pragma once
#include "victor.h"
#include <utility>


/// CONSTRUCTORS

template<class T>
victor<T>::victor() : len(0), capacity(0), arr(nullptr) {
}

template<class T>
victor<T>::victor(T *arr, unsigned long len) : len(len), capacity(len) {
    this->arr = new T[capacity];
    for (auto i = 0; i < len; i++)
        this->arr[i] = arr[i];
}

template<class T>
victor<T>::victor(const victor &obj) : len(obj.len), capacity(obj.capacity) {
    arr = new T[capacity];
    for (auto i = 0; i < len; i++)
        this->arr[i] = obj.arr[i];
}

template<class T>
victor<T>::victor(victor &&obj) : len(obj.len), capacity(obj.capacity), arr(obj.arr) {
    obj.arr = nullptr;
}

/// DESTRUCTOR

template <class T>
victor<T>::~victor() {
    delete[] arr;
}

/// OVERRIDING OPERATORS

template <class T>
victor<T>& victor<T>::operator= (const victor<T> &obj) {
    if (this == &obj)
        return *this;

    delete[] arr;
    victor tmp(obj);
    swap(tmp);
    return *this;
}

template <class T>
victor<T>& victor<T>::operator= (victor<T> &&obj) {
    if (this == &obj)
        return *this;

    swap(obj);
    return *this;
}

/// METODS

template <class T>
T* victor<T>::begin() {
    return arr;
}

template <class T>
T* victor<T>::end() {
    return (arr + len);
}

template <class T>
unsigned long victor<T>::size() {
    return len;
}

template <class T>
void victor<T>::increaseLen() {
    T* temp = new T[2*capacity];
    for (auto i = 0; i < len; i++)
        temp[i] = std::move(arr[i]);
    delete[] arr;
    arr = temp;
    capacity = 2*capacity;
}

template <class T>
void victor<T>::push_back(const T& el)
{
    if (len == 0)
    {
     arr = new T[1];
     len++;
     capacity++;
    }
    if (len == capacity)
        increaseLen();
    arr[len] = el;
    len++;
}

template <class T>
void victor<T>::push_back(T&& el)
{
    if (len == 0)
    {
        arr = new T[1];
        len++;
        capacity++;
    }
    if (len == capacity)
        increaseLen();
    arr[len] = std::move(el);
    len++;
}

template <class T>
void victor<T>::pop_back()
{
    erase(end() - 1);
}

template <class T>
void victor<T>::erase(const T* inx) {
    T *narr = new T[len - 1];
    for (auto i = arr; i != inx; i++)
        *(narr + (i - arr)) = *i;
    for (auto i = inx + 1; i != arr + len; i++)
        *(narr + (i - arr - 1)) = *i;
    delete[] arr;
    arr = narr;
    len--;
}

template <class T>
void victor<T>::erase(const T* b, const T* e) {
    if (e > b) {
        auto declen = e - b + 1;
        T *narr = new T[len - declen];
        for (auto i = arr; i != b; i++)
            *(narr + (i - arr)) = *i;
        for (auto i = e + 1; i != arr + len; i++)
            *(narr + (i - arr - declen)) = *i;
        delete[] arr;
        arr = narr;
        len -= declen;
    }
}

template <class T>
void victor<T>::swap(victor<T> &obj) {
    std::swap(arr, obj.arr);
    std::swap(capacity, obj.capacity);
    std::swap(len, obj.len);
}

template <class T>
void swap(victor<T> &v, victor<T> &w){
    v.swap(w);
}