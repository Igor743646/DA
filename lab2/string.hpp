#include <iostream>

#pragma once

namespace NMystd {
	struct TString {
		char str[256] {'\0'};
		int sizeString = 0;

		TString() = default;

		TString(const char* c) {
			while (c[sizeString] != '\0') {
				str[sizeString] = c[sizeString];
				++sizeString;
			}
		}

		TString(const TString& c) {
			*this = c;
		}

		~TString() = default;

		int Size() {
			return sizeString;
		}

		TString& operator=(const TString& rvl) {
			sizeString = rvl.sizeString;
			for (int i=0; (i!=256)&&(i<sizeString+1); ++i) {
				str[i] = rvl.str[i];
			}
			return *this;
		}

		TString& operator=(const char* c) {
			sizeString = 0;
			while (c[sizeString] != '\0') {
				str[sizeString] = c[sizeString];
				++sizeString;
			}
			str[sizeString] = c[sizeString];
			return *this;
		}

		bool operator==(const TString& rvl) const {
			if (sizeString != rvl.sizeString) return false;
			for (int i=0; i<sizeString; ++i) {
				if (str[i]!=rvl.str[i]) return false;
			}
			return true;
		}

		bool operator!=(const TString& rvl) const {
			return !(*this==rvl);
		}

		bool operator<(const TString& rvl) const {
			int s = sizeString < rvl.sizeString ? sizeString : rvl.sizeString;
			for (int i = 0; i < s; ++i) {
		        if (str[i] != rvl.str[i]) {
		            return str[i] < rvl.str[i];
		        }
		    }
		    return sizeString < rvl.sizeString;
		}

		bool operator>(const TString& rvl) const {
			return !(*this<rvl)&&(*this!=rvl);
		}

		char& operator[](const int i) {
			if (i < sizeString) return str[i];
			throw "ERROR"; 
		}
		
	};
}

std::istream& operator>>(std::istream& in, NMystd::TString& a) {
	a.str[0]='\0';
	in >> a.str;
	a.sizeString = 0;
	while (a.str[a.sizeString]!='\0') {
		++a.sizeString;
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const NMystd::TString& a) {
	out << a.str;
	return out;
}