#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

template<class T, class S>
void print(T* objects, S N) {
	for (S i = 0; i < N; i++) {
		cout << objects[i] << " ";
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

int main() {

	size_t n = 0;
	cin >> n;

	short* numbers = new short[n];
	size_t one_two_three[3] = {0, 0, 0};

	for (size_t i = 0; i < n; i++) {
		cin >> numbers[i];
		one_two_three[numbers[i]-1]++;
	}

	//print(numbers, n);
	//print<size_t, size_t>(one_two_three, 3);
	
	size_t result = 0;
	for (size_t i = 0; i < n; i++) {

		if (i < one_two_three[0]) { // если блок 1

			if (numbers[i] == 2) {
				for (size_t j = one_two_three[0]; j < n; j++) {
					if (numbers[j] == 1) {
						numbers[i] = 1;
						numbers[j] = 2;
						result++;
						break;
					}
				}
			} else if (numbers[i] == 3) {
				for (size_t j = one_two_three[0] + one_two_three[1]; j < n; j++) {
					if (numbers[j] == 1) {
						numbers[i] = 1;
						numbers[j] = 3;
						result++;
						break;
					}
				}

				if (numbers[i] == 3) {
					for (size_t j = one_two_three[0]; j < one_two_three[0] + one_two_three[1]; j++) {
						if (numbers[j] == 1) {
							numbers[i] = 1;
							numbers[j] = 3;
							result++;
							break;
						}
					}
				}
			}

		} else if (i < one_two_three[0] + one_two_three[1]) { // если блок 2

			if (numbers[i] == 3) {
				for (size_t j = one_two_three[0] + one_two_three[1]; j < n; j++) {
					if (numbers[j] == 2) {
						numbers[i] = 2;
						numbers[j] = 3;
						result++;
						break;
					}
				}
			}

		}

	}

	cout << result << endl;

	delete[] numbers;
	return 0;
}