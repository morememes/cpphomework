#include <iostream>
#include <vector>
#include <algorithm>  
#include <iterator>
#include <map>
#include <unordered_map>
#include <array>
#include <set>
#include <unordered_set>

template<class T>
class MyOsIterator {
	std::ostream *os;
	char separator;
	int divider;
	int counter = 0;
public:
	MyOsIterator(std::ostream &os, int divider = 1, char c = ' ') : os(&os), separator(c), divider(divider) {}
	MyOsIterator<T>& operator=(const T& el){
		if (counter % divider == 0) {
			*os << el;
			if (separator)
				*os << separator;
		}
		return *this;
	}
	MyOsIterator<T>& operator++(){
		counter++;
		return *this;
	}
	MyOsIterator<T>& operator++(int) {
		counter++;
		return *this;
	}
	MyOsIterator<T>& operator*(){
		return *this;
	}
};

class Person
{
	int id;
	std::string name;
public:
	Person(int id, std::string name) : id(id), name(name) {}
	int getID() const {
		return id;
	}
};

bool operator<(const Person& a, const Person& b) {
	return a.getID() < b.getID();
}

bool operator==(const Person& a, const Person& b) {
	return a.getID() == b.getID();
}

namespace std {
	// объявление шаблона
	template <class T> struct hash;
	// частичная специализация для примитивных типов
	template <>
	struct hash<Person> {
		typedef size_t result_type;
		typedef Person argument_type;
		result_type operator()(argument_type x) const {
			return static_cast<result_type>(x.getID());
		}
	};
}

template <class Iter>
void MergeSort(Iter b, Iter e) {
	typedef typename Iter::value_type type;
	if ((e - b) > 3) {
		MergeSort(b, b + (e - b) / 2);
		MergeSort(b + (e - b) / 2, e);
	}
	Iter sep = b + (e - b) / 2;
	Iter b1 = b + (e - b) / 2;
	Iter bc = b;
	const size_t size_arr = e - b;
	type *a = new type[size_arr];
	//std::array<Iter::value_type, size_arr> a;
	int i = 0;
	while (true) {
		if (*b > *b1)
			a[i] = *b1++;
		else
			a[i] = *b++;
		i++;
		if (b == sep || b1 == e)
			break;
	}
	if (b == sep) {
		while (b1 != e) {
			a[i] = *b1++;
			i++;
		}
	}
	else
	{
		while (b != sep) {
			a[i] = *b++;
			i++;
		}
	}
	i = 0;
	while (bc != e) {
		*bc = a[i];
		bc++;
		i++;
	}
}


int main()
{
	MyOsIterator<int> MyOs(std::cout);
	std::vector<int> vec = { 5,4,3,2,1,6,7,8,9 };
	MergeSort(vec.begin(), vec.end());
	std::copy(vec.begin(), vec.end(), MyOs);
	std::cout << std::endl;
	std::array<float, 7>  arr = { 4.0f, 2.0f, 2.0f, 1.9f, 5.5f, 3.2f, 1.0f };
	MergeSort(arr.begin(), arr.end());
	std::copy(arr.begin(), arr.end(), MyOs);

	std::cout << std::endl;

	std::map<Person, int> map;
	std::unordered_map<Person, int> umap;
	Person a1(1, "Vasya"), a2(2, "Nikita");
	map[a1] = 14;
	umap[a2] = 19;
	std::cout << map[a1] << " " << umap[a2] << std::endl;
	std::set<Person> set;
	std::unordered_set<Person> uset;
	set.insert(a2); uset.insert(a1);
	std::cout << (*set.begin()).getID() << std::endl;
	std::cout << (*uset.begin()).getID();
	return 0;
}
