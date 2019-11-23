#include "pch.h"
#include <iostream>
#include <complex>
#include <string>
#include <vector>

template <typename T>
struct Side{
	T x;
	T y;
};

template <typename T>
class rectangle{
	Side<T> side;
public:
	Side<T>& getSide() {
		return side;
	}
	Side<T> getSide() const {
		return side;
	}
};

template <typename T>
class circle {
	T rad;
public:
	T& getRad() {
		return rad;
	}
	T getRad() const {
		return rad;
	}
};

template <class T>
class Trait {
public:
	static size_t size(T &fig){
		return sizeof(fig);
	}
};

template <class T>
class Trait<circle<T>> {
public:
	static size_t size(circle<T> &crc) {
		std::cout << "Privet" << std::endl;
		return sizeof(crc);
	}
};

template <class T>
class Trait<rectangle<T>> {
public:
	static size_t size(rectangle<T> &rct) {
		std::cout << "Zdarova" << std::endl;
		return sizeof(rct);
	}
};





template<class T>
class Polynomial {
	std::vector<T> polinom;
	int degree;
public:
	Polynomial(std::initializer_list<T> list) : degree(list.size()) {
		for (auto &el : list)
			polinom.push_back(el);
	}
	Polynomial(std::vector<T> vec) : degree(vec.size()) {
		for (auto &el : vec)
			polinom.push_back(el);
	}

	std::vector<T> getPoly() const{
		return polinom;
	}
};

template<class T>
std::ostream& operator<<(std::ostream &os, Polynomial<T> const& p) {
	std::vector<T> poly = p.getPoly();
	for (int i = poly.size() - 1; i != 0; i--)
		os << poly[i] << " * x^" << i << " + ";
	os << poly[0];
	return os;
}

template<class T>
Polynomial<T> operator*(Polynomial<T> &poly, T d) {
	std::vector<T> pol = poly.getPoly();
	for (auto &el : pol)
		el *= d;
	return Polynomial<T>(pol);
}

template<class T>
Polynomial<T> operator*(T d, Polynomial<T> &poly) {
	return poly * d;
}

int main()
{
	Polynomial<float> pol = { 9.0, 6.0, 1.0 };
	pol = 2.0f * pol;
	std::cout << pol;
	return 0;
}