/*
    Глушатов Игорь Сергеевич
    Группа: М80-207Б-19

    Требуется разработать программу, осуществляющую ввод пар «ключ-значение», их упорядочивание по возрастанию ключа 
    указанным алгоритмом сортировки за линейное время и вывод отсортированной последовательности.
    Вариант задания определяется типом ключа (и соответствующим ему методом сортировки) и типом значения:

    Поразрядная сортировка.

    Тип ключа: числа от 0 до 264 - 1.

    Тип значения: строки фиксированной длины 64 символа, во входных данных могут встретиться строки меньшей длины, 
    при этом строка дополняется до 64-х нулевыми символами, которые не выводятся на экран.
*/

#include <iostream>
#include <cmath>

const int BASIC_VECTOR_BUFFER = 128;
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
    for (int i = STRING_SIZE; i < 64; ++i) value[i] = '\0';
}

template<typename T>
class TVector {
    private:
        int size;
        int buffer;
        T *massive;
    public:
        TVector();
        TVector(const long lenth);
        ~TVector();
        void Append(const T &element);
        int Len();
        T& operator[](const long id);
        TVector<T>& operator=(TVector<T> other);
};

template<typename T>
TVector<T>::TVector(): size(0), buffer(0), massive(0) {}

template<typename T>
TVector<T>::TVector(const long lenth): size(0), buffer(lenth) {
    massive = new T[lenth];
}

template<typename T>
TVector<T>::~TVector() {
    delete []massive;
}

template<typename T>
void TVector<T>::Append(const T &element) {
    if (buffer == 0) {
        massive = new T[BASIC_VECTOR_BUFFER];
        buffer = BASIC_VECTOR_BUFFER;
    } else {
        if (size == buffer) {
            T *temporaryMassive = new T[buffer << 1];
            buffer<<=1;
            for (long i = 0; i < size; ++i) {
                temporaryMassive[i] = massive[i];
            }
            delete []massive;
            massive = temporaryMassive;
        }
    }
    massive[size] = element;
    size++;
}

template<typename T>
int TVector<T>::Len() {
    return size;
}

template<typename T>
T& TVector<T>::operator[](const long id) {
    return massive[id];
}

int Rank(const unsigned long long number, int i) {
    return (number >> 4*i)&15;
}

void RadixSort(TVector<TPair> &massive) {
    if (massive.Len()==0) {
        return;
    }

    TVector<TPair> res(massive.Len());
    
    for (long j = 0; j < 16; ++j) {

        int count[16] {0}; 

        for (long i = 0; i<massive.Len(); ++i) {
            ++count[Rank(massive[i].key, j)];
        }

        if (count[0] == massive.Len()) {
            continue;
        }

        for (long i = 1; i<16; ++i) {
            count[i]+=count[i-1];
        }

        for (long i = massive.Len()-1; i>=0; --i) {
            res[count[Rank(massive[i].key, j)]-1] = massive[i];
            --count[Rank(massive[i].key, j)];
        }

        for (int i=0; i < massive.Len(); ++i) {
            massive[i] = res[i];
        }
    }
}


int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    TVector<TPair> vec;
    unsigned long long key; 
    char value[STRING_SIZE];

    while (std::cin >> key >> value) {
        vec.Append(TPair(key, value));
    }

    RadixSort(vec);

    for (long i = 0; i < vec.Len(); ++i) {
        std::cout << vec[i].key << " " << vec[i].value << "\n";
    }

    return 0;
} 