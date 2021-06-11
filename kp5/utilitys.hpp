#include <iostream>
#include <vector>

using namespace std;

template<class T>
vector<T> range(T l, T r, T step = 1) {
	vector<T> result;
	for (T i = l; i < r; i += step) {
		result.push_back(i);
	}
	return result;
}

template<class T>
void print(vector<T> objects) {

	cout << "[";

	size_t i = objects.size();
	for (const T& object : objects) {
		cout << object;
		i--;
		cout << (i > 0 ? ", " : "");
	}

	cout << "]" << endl;
}

template<class T>
void print(initializer_list<T> objects) {
	for (const T& object : objects) {
		cout << object << " ";
	}

	cout << endl;
}

template<class T>
void print(T object, bool end = true) {
	if (end == true)
		cout << object << endl;
	else
		cout << object << " ";
}

template<class T, class... Args>
void print(T& first, Args... next) {
	print(first, false);
	print(next...);
}