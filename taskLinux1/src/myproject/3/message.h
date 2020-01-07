#include <iostream>
#include <string>
#include <fstream>

template <class Head>
void message(std::ostream& out, const char *str, Head head){
    while (*str){
        if (*str == '%')
            out << head;
        else
            out << *str;
        str++;
    }
}

template <typename Head, typename ... Tail>
void message(std::ostream& out, const char * str, Head head, Tail  ... tail){
    while (*str){
        if (*str == '%'){
            out << head;
            return message(out, ++str, tail ...);
        }else
            out << *str++;
    }
}

