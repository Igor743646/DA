#ifndef PAIR_HPP
#define PAIR_HPP
const int STRING_SIZE = 64;

struct TPair {
	unsigned long long key;
	char value[STRING_SIZE];
	TPair();
	TPair(const unsigned long long k, const char v[STRING_SIZE]);
};

bool operator<(const TPair &lhs, const TPair &rhs) {
	return lhs.key < rhs.key;
}

TPair::TPair(): key(0) {
	for (int i = 0; i < STRING_SIZE; ++i) value[i] = '\0';
}

TPair::TPair(const unsigned long long k, const char v[STRING_SIZE]): key(k) {
	for (int i = 0; i < STRING_SIZE; ++i) value[i] = v[i];
}


#endif