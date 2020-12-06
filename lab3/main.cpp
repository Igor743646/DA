#include <iostream>
#include <chrono>
#include <fstream>
#include "avltree.hpp"
#include "string.hpp"

void ToLower(NMystd::TString *str) {
	for (int i=0; i<str->Size(); ++i) {
		if (str->str[i]>='A' && str->str[i]<='Z') {
			str->str[i] = (str->str[i] - ('A'-'a'));
		}
	}
}

int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    // std::chrono::time_point<std::chrono::system_clock> start1 = std::chrono::system_clock::now();
    // std::chrono::time_point<std::chrono::system_clock> end1 = std::chrono::system_clock::now();
    unsigned int count_of_comands = 0;
    std::chrono::duration<double> diff(0);
    
	NMystd::TString c;
	TAVLTree *tree = 0;
	
	while (std::cin >> c) {
		
		if (c == "+") {

			unsigned long long value;
			std::cin >> c >> value;
			ToLower(&c);
			tree = tree->Insert(tree, c, &value);

		} else if (c == "-") {

			std::cin >> c;
			ToLower(&c);
			tree = tree->Remove(tree, c);
			
		} else if (c == "!") {
			std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
			std::cin >> c;
			if (c == "Save") {
				std::cin >> c;
				FILE *op = fopen(c.str, "w");
				if (op == nullptr) {
					std::cout << "ERROR: can not open file\n";
				} else {
					tree->Save(tree, op);
					fclose(op);
					std::cout << "OK\n";
				}
				//tree->Print(tree, 0);
			} else {

				std::cin >> c;
				std::ifstream in(c.str);
				//FILE *op = fopen(c.str, "r");
				if (!in.is_open()) {
					std::cout << "ERROR: can not open file\n";
				} else {

					tree->RemoveTree(tree);
					tree = tree->Load(in);
					in.close();
					std::cout << "OK\n";
					//tree->Print(tree, 0);
					
				}
			}
			std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
			diff=(end-start);
		} else {
			ToLower(&c);
			tree->Search(tree, c);
		}
		count_of_comands++;
		
	}
	tree->RemoveTree(tree);
	

	std::ofstream fout("time_log.txt", std::ios_base::app);
	fout << count_of_comands << " " << diff.count() << "\n";
	fout.close();
	return 0;
}