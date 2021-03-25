#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

//#define DEBAG
#define BENCH

#ifdef DEBAG
    #include <chrono>
    #include <fstream>
#endif

namespace Calculater {
    class BigInt {

        static const int32_t BASE = 10000;
        static const int32_t RADIX = 4;

    private:
        std::vector<int32_t> _data;

    public:

        BigInt() = default;

        BigInt(const std::string& str) {
            for (size_t i = str.size(); i > 0; i -= BigInt::RADIX) {
                if (i < BigInt::RADIX) {
                    _data.push_back(atoi(str.substr(0, i).c_str()));
                    break;
                } else {
                    _data.push_back(atoi(str.substr(i - BigInt::RADIX, BigInt::RADIX).c_str()));
                }     
            }

            RemoveZeros();
        }

        BigInt(const int32_t num) {
            _data.push_back(num % BigInt::BASE);
            _data.push_back((num / BigInt::BASE) % BigInt::BASE);
            _data.push_back((num / BigInt::BASE) / BigInt::BASE);

            RemoveZeros();
        }

        void RemoveZeros() {
            while (_data.size() > 1 && _data.back() == 0) {
                _data.pop_back();
            }
        }

        BigInt operator+(const BigInt& a) const {
            BigInt res;
            int32_t carry = 0;
            size_t n = std::max(a._data.size(), _data.size());
            for (size_t i = 0; i < n; ++i) {
                int32_t sum = carry;
                if (i < a._data.size()) {
                    sum += a._data[i];
                }
                if (i < _data.size()) {
                    sum += _data[i];
                }
                carry = sum / BigInt::BASE;
                res._data.push_back(sum % BigInt::BASE);
            }
            if (carry != 0) {
                res._data.push_back(1);
            }
            res.RemoveZeros();
            return res;
        }

        BigInt operator-(const BigInt& a) const {
            BigInt res;
            int32_t p, carry = 0;
            size_t n = std::max(a._data.size(), _data.size());
            for (size_t i = 0; i < n; ++i) {
                p = _data[i] - carry;
                carry = 0;

                if (i >= a._data.size()) {
                    if (p < 0) {
                        carry = 1;
                        p += BigInt::BASE;
                    }
                    res._data.push_back(p);
                    continue;
                }

                if (p < a._data[i]) {
                    carry = 1;
                    p += BigInt::BASE;
                }

                res._data.push_back(p - a._data[i]);
            }

            res.RemoveZeros();
            return res;
        }

        BigInt operator*(const BigInt& a) const {
            BigInt res;
            size_t n = a._data.size() + _data.size();
            res._data.resize(n);

            int32_t k = 0;
            int32_t r = 0;
            for (size_t i = 0; i < _data.size(); i++) {
                for (size_t j = 0; j < a._data.size(); j++) {
                    k = _data[i] * a._data[j] + res._data[i + j];
                    r = k / BigInt::BASE;
                    res._data[i + j + 1] = res._data[i + j + 1] + r;
                    res._data[i + j] = k % BigInt::BASE;
                }
            }

            res.RemoveZeros();
            return res;
        }

        bool Odd(const BigInt& a) const {
            return a._data[0] & 1;
        }

        BigInt Pow(BigInt a, BigInt b) const {
            BigInt res(1);
            while (b > BigInt(0)) {
                if (Odd(b)) {
                    res = res * a;
                }
                a = a * a;
                b = b / BigInt(2);
            }
            return res; 
        }

        BigInt operator*(const int32_t& a) const {
            BigInt c(a);
            return c * (*this);
        }

        int32_t FindFactor(const BigInt& cur, const BigInt& a) const {
            if (cur < a) return 0;
            
            int32_t l = 1;
            int32_t r = BigInt::BASE - 1;

            while (l <= r and l+1 != r) {
                int32_t m = (l + r) / 2;
                BigInt mm = a * m;

                if (cur == mm) {
                    return m;
                } else if (cur < mm) {
                    r = m;
                } else {
                    l = m;
                }
            }
            if (a*r <= cur) {
                return r;
            } else {
                return l;
            }
        }

        BigInt operator/(const BigInt& a) const {
            BigInt res;
            
            if (*this == a) return BigInt(1);
            if (*this < a) return BigInt(0);

            if (_data.size() == a._data.size()) {
                return BigInt(FindFactor(*this, a));
            } else {
                BigInt cur;
                cur._data = {_data.begin() + (_data.size() - a._data.size()), _data.end()};
                int32_t inc = _data.size() - a._data.size();

                if (cur < a) {
                    cur._data.insert(cur._data.begin(), _data[inc - 1]);
                    inc--;
                }
                do {
                    int32_t num = FindFactor(cur, a);
                    res._data.insert(res._data.begin(), num);
                    cur = cur - (a * num);
                    if (inc == 0) {
                        break;
                    } else {
                        inc--;
                        cur._data.insert(cur._data.begin(), _data[inc]);
                    }
                    cur.RemoveZeros();
                } while(true);
            }
            
            return res;
        }

        bool operator<(const BigInt& a) const {
            if (_data.size() != a._data.size()) {
                return _data.size() < a._data.size();
            }
            else {
                for (int32_t i = _data.size() - 1; i >= 0; i--) {
                    if (_data[i] != a._data[i]) {
                        return _data[i] < a._data[i];
                    }
                }
                return false;
            }
        }

        bool operator<=(const BigInt& a) const {
            if (_data.size() != a._data.size()) {
                return _data.size() < a._data.size();
            }
            else {
                for (int32_t i = _data.size() - 1; i >= 0; i--) {
                    if (_data[i] != a._data[i]) {
                        return _data[i] < a._data[i];
                    }
                }
                return true;
            }
        }

        bool operator>(const BigInt& a) const {
            if (_data.size() != a._data.size()) {
                return _data.size() > a._data.size();
            }
            else {
                for (int32_t i = _data.size() - 1; i >= 0; i--) {
                    if (_data[i] != a._data[i]) {
                        return _data[i] > a._data[i];
                    }
                }
                return false;
            }
        }

        bool operator==(const BigInt& a) const {
            if (_data.size() != a._data.size()) {
                return false;
            }
            else {
                for (size_t i = 0; i < _data.size(); i++) {
                    if (_data[i] != a._data[i]) {
                        return false;
                    }
                }
                return true;
            }
        }

        friend std::istream& operator>> (std::istream& in, BigInt& a) {
            std::string str;
            in >> str;
            a._data.clear();
            for (size_t i = str.size(); i > 0; i -= BigInt::RADIX) {
                if (i < BigInt::RADIX) {
                    a._data.push_back(atoi(str.substr(0, i).c_str()));
                    break;
                }
                else {
                    a._data.push_back(atoi(str.substr(i - BigInt::RADIX, BigInt::RADIX).c_str()));
                }
            }

            a.RemoveZeros();
            return in;
        }

        friend std::ostream& operator<< (std::ostream& out, const BigInt& a) {
            out << a._data.back();
            for (int32_t i = a._data.size() - 2; i >= 0; i--) {
                out << std::setfill('0') << std::setw(BigInt::RADIX) << a._data[i];
            }
            return out;
        }
    };
}

int main() {
    Calculater::BigInt a, b;
    std::string comand;

#ifdef DEBAG
    //size_t radmax = 0;
    std::chrono::time_point<std::chrono::system_clock> start1 = std::chrono::system_clock::now();
#endif

    while (std::cin >> a >> b >> comand) {

        //if (a._data.size() > radmax or b._data.size() > radmax) radmax = std::max(a._data.size(), b._data.size());

        if (comand == "+") {
            std::cout << a + b << std::endl;
        }
        else if (comand == "-") {
            if (a < b) {
                std::cout << "Error" << std::endl;
                continue;
            }
            std::cout << a - b << std::endl;
        }
        else if (comand == "*") {
            std::cout << a * b << std::endl;
        }
        else if (comand == "/") {
            if (b == Calculater::BigInt(0)) {
                std::cout << "Error" << std::endl;
                continue;
            }
            std::cout << a / b << std::endl;
        }
        else if (comand == "^") {
            if (a == Calculater::BigInt(0) and b == Calculater::BigInt(0)) {
                std::cout << "Error" << std::endl;
                continue;
            }
            std::cout << a.Pow(a, b) << std::endl;
        }
        else if (comand == "<") {
            std::cout << (a < b ? "true" : "false") << std::endl;
        }
        else if (comand == ">") {
            std::cout << (a > b ? "true" : "false") << std::endl;
        }
        else if (comand == "=") {
            std::cout << (a == b ? "true" : "false") << std::endl;
        }
    }

#ifdef DEBAG
    std::chrono::time_point<std::chrono::system_clock> end1 = std::chrono::system_clock::now();

    std::ofstream fout("time_log.txt", std::ios_base::app);
    fout << std::chrono::duration<double>(end1-start1).count() << "\n";
    fout.close();
#endif

    return 0;
}