#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include "point.hpp"
#include "edge.hpp"
#include "persistent_tree2.hpp"

// #define PRINT
// #define TREE
#define LOG

using namespace std;

struct Index {

	PersistentTree<Edge, unsigned int> tree;
	vector<double> slabs;

	Index() = default;

	Index(vector<vector<Point>> figures) {

		vector<pair<Edge, unsigned int>> edges;
		vector<Edge> for_enter;

#ifdef PRINT
		print("[");
		for (int i = 0; i < figures.size(); i++){
			print(" ", figures[i]);
		}
		print("]");
#endif
		// Создание массива рёбер
		for (vector points : figures) {
			static unsigned int figure_number = 1;
			for (size_t i = 0; i < points.size(); i++) {
				if (points[i].x == points[(i+1) % points.size()].x) continue;
				edges.push_back(pair<Edge, unsigned int> (Edge(points[i], points[(i+1) % points.size()]), figure_number) );
				for_enter.push_back(Edge(points[i], points[(i+1) % points.size()]));
			}
			figure_number++;
		}

		// Создание массива плит
		for (vector points : figures) {
			for (const Point p : points) {
				if (find(slabs.begin(), slabs.end(), p.x) == slabs.end())
					slabs.push_back(p.x);
			}
		}

		// Сортировка массива плит, ребер на вход и на выход
		sort(slabs.begin(), slabs.end());
		sort(edges.begin(), edges.end(), [](const pair<Edge, unsigned int>& p1, const pair<Edge, unsigned int>& p2) {
			return p1.first.org.x < p2.first.org.x;
		});
		sort(for_enter.begin(), for_enter.end(), [](const Edge& p1, const Edge& p2) {
			return p1.dest.x < p2.dest.x;
		});

		bool flag = false;
		for (int i = 0; i < slabs.size(); i++) {

			// Пока есть отрезки, которые на предыдуще1 плите закончились - удаляем
			while (!for_enter.empty() and for_enter[0].dest.x == slabs[i] and i>0) {  
				tree.Remove(for_enter[0], slabs[i-1] + 10E-9, flag);
				for_enter.erase(for_enter.begin());

				if (!flag)
					flag = true;
			}

			// Пока есть отрезки, которые начинаются в данной плите - добавляем 
			while (!edges.empty() and edges[0].first.org.x == slabs[i]) {
				tree.Insert(edges[0].first, edges[0].second, slabs[i] + 10E-9, flag);
				edges.erase(edges.begin());

				if (!flag)
					flag = true;
			}

			// Если никакие не добавились и не закончились
			if (!flag)
				tree.NotChange();

			flag = false;
		}
#ifdef PRINT
#ifdef TREE
		tree.Print();
#endif
#endif
	}

	unsigned int NumberOfEdgesAbovePoint(Point p, long* la, long* ra) {

		if (p.x < slabs[0] or p.x > slabs[slabs.size()-1])
			return 0;
		unsigned int l = 0, r = slabs.size();

		bool flag = false;
		while(r  - l > 1) {

			unsigned int mid = (l + r) / 2;

			if (slabs[mid] == p.x) {
				l = mid;
				flag = true; // если точка на границе, то корень нечетный
				break;
			}

			if (p.x < slabs[mid]) {
				r = mid;
			} else {
				l = mid;
			}

		}
		
		long left_ancestor = -1, right_ancestor = -1;
		unsigned int res = tree.FindNumberAbove(l > 0 ? l-flag : l, p, &left_ancestor, &right_ancestor);

		*la = left_ancestor; 
		*ra = right_ancestor;
		return res;
	}

	long inWhichPoligone(Point p) {

		long left_ancestor = -1, right_ancestor = -1;
		unsigned int noeap = NumberOfEdgesAbovePoint(p, &left_ancestor, &right_ancestor);

		if (noeap % 2 == 1) {
			return left_ancestor;
		}

		return -1;
	}

	void Write(string name_of_file) {
		ofstream save_file;
		save_file.open(name_of_file);

		if (!save_file.is_open()) {
			print("Bad record");
		}

		save_file << slabs.size() << "\n";
		for (const double& p : slabs) {
			save_file << p << " ";
		}
		save_file << "\n";
		save_file << tree << "\n";

		save_file.close();
	}

	void Read(string name_of_file) {
		ifstream save_file;
		save_file.open(name_of_file);

		if (!save_file.is_open()) {
			print("Bad reading");
		}

		unsigned int size_of_slabs;
		save_file >> size_of_slabs;

		for (size_t i = 0; i < size_of_slabs; i++) {
			double slab;
			save_file >> slab;
			slabs.push_back(slab);
		}

		save_file >> tree;
		save_file.close();
	}

};


void Read(vector<vector<Point>>* figures, istream& from) {
	unsigned int number_of_figures;
	from >> number_of_figures;

	for (const unsigned int i : range<unsigned int>(0, number_of_figures)) {
		unsigned int number_of_vertexes;
		from >> number_of_vertexes;

		figures->push_back(vector<Point>());

		for(const unsigned int j : range<unsigned int>(0, number_of_vertexes)) {
			double l, r;
			from >> l >> r;
			(*figures)[i].push_back(Point(l, r));
		}
	}
}


int main(int args, char** argv) {
	vector<vector<Point>> figures;

#ifdef LOG
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
	ofstream log_file;
	log_file.open("log1.txt", std::fstream::app);
#endif

	if (args == 1) {
		print("Не введен один из ключей: index, search");
	}

	else if (args == 2 and string(argv[1]) == "index") {
		
		Read(&figures, cin);

		Index Poligones(figures);
		Poligones.Write("base_output.txt");
		print("Структура построена и сохранена в файле: base_output.txt");
	}

	else if (args == 4 and string(argv[1]) == "index" and string(argv[2]) == "--input") {

		ifstream file;
		file.open(argv[3]);

		if (!file.is_open()) {
			print("Bad reading");
			return 0;
		}

		Read(&figures, file);

#ifdef LOG
		start = std::chrono::system_clock::now();
		Index Poligones(figures);
		end = std::chrono::system_clock::now();
		Poligones.Write("base_output.txt");
		log_file << figures[0].size() << " " << std::chrono::duration<double>(end-start).count() << "\n";
#else
		Index Poligones(figures);
		Poligones.Write("base_output.txt");
#endif
		print("Структура считана из файла:", argv[3], "и сохранена в файле: base_output.txt");

		file.close();
	}

	else if (args == 4 and string(argv[1]) == "index" and string(argv[2]) == "--output") {

		Read(&figures, cin);

		Index Poligones(figures);
		Poligones.Write(argv[3]);
		print("Структура построена и сохранена в файле:", argv[3]);
	}

	else if (args == 6 and string(argv[1]) == "index" and string(argv[2]) == "--input" and string(argv[4]) == "--output") {

		ifstream file;
		file.open(argv[3]);

		if (!file.is_open()) {
			print("Bad reading");
			return 0;
		}

		Read(&figures, file);

#ifdef LOG
		start = std::chrono::system_clock::now();
		Index Poligones(figures);
		end = std::chrono::system_clock::now();
		Poligones.Write(argv[5]);
		log_file << figures[0].size() << " " << std::chrono::duration<double>(end-start).count() << "\n";
#else
		Index Poligones(figures);
		Poligones.Write(argv[5]);
#endif
		print("Структура считана из файла:", argv[3], "и сохранена в файле:", argv[5]);

		file.close();
	}

	else if (args == 2 and string(argv[1]) == "search") {
		Read(&figures, cin);
		Index Poligones(figures);

		double x,y;
		while (cin >> x >> y) {
			cout << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}
	}

	else if (args == 4 and string(argv[1]) == "search" and string(argv[2]) == "--input") {
		Read(&figures, cin);
		Index Poligones(figures);

		ifstream file;
		file.open(argv[3]);
		
		if (!file.is_open()) {
			print("Bad reading");
			return 0;
		}

		double x,y;
		while (file >> x >> y) {
			cout << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}

		file.close();
	}

	else if (args == 4 and string(argv[1]) == "search" and string(argv[2]) == "--index") {
		Index Poligones;
		Poligones.Read(string(argv[3]));
	
		double x,y;
		while (cin >> x >> y) {
			cout << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}

	}

	else if (args == 4 and string(argv[1]) == "search" and string(argv[2]) == "--output") {
		Read(&figures, cin);
		Index Poligones(figures);
	
		ofstream file;
		file.open(argv[3]);
		
		if (!file.is_open()) {
			print("Bad opening");
			return 0;
		}

		double x,y;
		while (cin >> x >> y) {
			file << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}

		file.close();
	}

	else if (args == 6 and string(argv[1]) == "search" and string(argv[2]) == "--index" and string(argv[4]) == "--input") {
		Index Poligones;
		Poligones.Read(string(argv[3]));

		ifstream file;
		file.open(argv[5]);

		if (!file.is_open()) {
			print("Bad opening");
			return 0;
		}

		double x,y;
		while (file >> x >> y) {
			cout << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}

		file.close();
	}

	else if (args == 8 and string(argv[1]) == "search" and string(argv[2]) == "--index" and string(argv[4]) == "--input" and string(argv[6]) == "--output") {
		Index Poligones;
		Poligones.Read(string(argv[3]));

		ifstream file;
		ofstream out_file;
		file.open(argv[5]);
		out_file.open(argv[7]);

		if (!file.is_open() or !out_file.is_open()) {
			print("Bad opening");
			return 0;
		}

#ifdef LOG
		start = std::chrono::system_clock::now();
		double x,y;
		unsigned int np = 0;
		while (file >> x >> y) {
			np++;
			out_file << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}
		end = std::chrono::system_clock::now();
		log_file << Poligones.slabs.size() << " " << std::chrono::duration<double>(end-start).count() << "\n";
#else
		double x,y;
		while (file >> x >> y) {
			out_file << Poligones.inWhichPoligone(Point(x, y)) << endl;
		}
#endif

		out_file.close();
		file.close();
	} else {
		print("Неправильный ввод");
	}

#ifdef LOG
	log_file.close();
#endif
	
	return 0;
}