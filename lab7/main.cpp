#include <iostream>
#include <string>
#include <algorithm>

int main() {

	unsigned int n;
	std::cin >> n;

	unsigned int* table = new unsigned int[n]();

	for (unsigned int ind = 1; ind < n; ind++) {
		unsigned int number = ind + 1;

		if (number % 2 == 0 and number % 3 == 0) {
			if (table[ind - 1] < table[ind / 2]) {
				if (table[ind - 1] < table[ind / 3]) {
					table[ind] = table[ind - 1] + number;
				} else {
					table[ind] = table[ind / 3] + number;
				}
			} else {
				if (table[ind / 2] < table[ind / 3]) {
					table[ind] = table[ind / 2] + number;
				} else {
					table[ind] = table[ind / 3] + number;
				}
			}
		} else if (number % 3 == 0) {
			if (table[ind - 1] < table[ind / 3]) {
				table[ind] = table[ind - 1] + number;
			} else {
				table[ind] = table[ind / 3] + number;
			}
		} else if (number % 2 == 0) {
			if (table[ind - 1] < table[ind / 2]) {
				table[ind] = table[ind - 1] + number;
			} else {
				table[ind] = table[ind / 2] + number;
			}
		} else {
			table[ind] = table[ind - 1] + number;
		}
	}

	std::string res = "";

	unsigned int last_n = n;
	unsigned int new_n = n;
	long int i = n - 1;
	unsigned int delta = table[i] - (i + 1);

	while (i >= 0) {
		if (delta == table[i]) {
			last_n = i + 1;

			if (last_n * 3 == new_n) {
				res += "/3 ";
			} else if (last_n * 2 == new_n) {
				res += "/2 ";
			} else if (last_n + 1 == new_n) {
				res += "-1 ";
			} else {
				i--;
				continue;
			}

			new_n = last_n;
			delta -= last_n;
		}

		i--;
	}

	std::cout << table[n-1] << std::endl;
	std::cout << res << std::endl;

	delete[] table;
	return 0;
}