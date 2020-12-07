#include <string>
#include <vector>
#include <stdio.h>

const short TLETTER_SIZE = 17; // Размер слова

struct TLetter {
    char letter[TLETTER_SIZE] {'\0'};
    unsigned int strIndex;
    unsigned int textIndex;

    TLetter() = default;

    TLetter(std::string* s, unsigned int si, unsigned int ti): strIndex(si), textIndex(ti) {
        for (unsigned int i = 0; i < s->size(); ++i) {
            letter[i] = (*s)[i];
        }
    };

    TLetter(std::string s, unsigned int si, unsigned int ti): strIndex(si), textIndex(ti) {
        for (unsigned int i = 0; i < s.size(); ++i) {
            letter[i] = s[i];
        }
    };

    ~TLetter() = default;

    const bool operator==(const TLetter &rval) const {
        for (unsigned int i = 0; i < TLETTER_SIZE; ++i) {
            if (letter[i] != rval.letter[i]) {
                return false;
            }
        }
        return true;
    } 
};

unsigned int* ZFunction(const std::vector<TLetter> pattern) {
    unsigned int patternSize = pattern.size();
    unsigned int* result = new unsigned int[patternSize] {0};
    unsigned int left = 0;
    unsigned int right = 0;
    for (unsigned int i = 1; i < patternSize; ++i) { // Проходимся по каждому терму
        if (i <= right) { // Индекс с которого можно начать сравнивать
            result[i] = std::min(right - i, result[i - left]);
        }
        while (i + result[i] < patternSize and pattern[result[i]] == pattern[i + result[i]]) { // Наивный подсчет, если result[i] == 0
            ++result[i];
        }
        if (i + result[i] > right) { // Смещение границы самого правого вхождения
            left = i;
            right = i + result[i];
        }
    }
    return result;
}

void ToLower(char* str) { // Перевод в нижний регистр символа
    if (*str>='A' and *str<='Z') {
        *str -= 'A'-'a';
    }
}

int main() {

    std::vector<TLetter> text; // Массив термов вида: pattern + "#" + text

    unsigned int strNumber = 0;
    unsigned int textNumber = 0;
    unsigned int patternSize = 0;

    std::string letter = "";
    char c;
    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            if (letter != "") {
                text.push_back(TLetter(&letter, strNumber, textNumber));
                ++textNumber;
                ++patternSize;
                letter = "";
            }
        } else if (c == '\n') {
            if (letter != "") {
                text.push_back(TLetter(&letter, strNumber, textNumber));
                ++textNumber;
                ++patternSize;
                letter = "";
            }
            break;
        } else {
            ToLower(&c);
            letter += c;
        }
    }

    if (!text.size()) { // Если паттерн пустой
        return 0;
    }

    text.push_back(TLetter(std::string("#"), strNumber, 0)); // Дополняем разделяющим термом
    strNumber = 1;
    textNumber = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            if (letter != "") {
                text.push_back(TLetter(&letter, strNumber, textNumber));
                ++textNumber;
                letter = "";
            }
        } else if (c == '\n') {
            if (letter != "") {
                text.push_back(TLetter(&letter, strNumber, textNumber));
                letter = "";
            }
            textNumber = 0;
            ++strNumber;
        } else {
            ToLower(&c);
            letter += c;
        }
    }

    if (letter != "") {
        text.push_back(TLetter(&letter, strNumber, textNumber));
    }

    unsigned int* zf = ZFunction(text); // Расчет Z - функции для массива вида: pattern + "#" + text

    for (unsigned int i = 0; i < text.size(); ++i) { // Вывод ответа
        if (zf[i] == patternSize) { // Если значение z - функции равно длине паттерна, то это вхождение
            printf("%d, %d\n", text[i].strIndex, text[i].textIndex+1);
        }
    }

    delete []zf;
    return 0;
}