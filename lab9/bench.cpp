#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std;

template<class T>
void print(set<T> objects) {
	for (const T& object : objects) {
		cout << object << " ";
	}

	cout << endl;
}

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

template<class T>
void print(vector<T> objects, bool end = false) {
	for (const T& object : objects) {
		print(object, end);
	}

	cout << endl;
}

template<class T, class... Args>
void print(T& first, Args... next) {
	print(first, false);
	print(next...);
}

class Graph {

	using vertex_type = size_t;

	public:
		unordered_map<vertex_type, set<vertex_type>> edges;

	Graph() = default;
	Graph(const size_t n) {
		for (size_t i = 1; i <= n; i++) 
			edges[i] = set<vertex_type>{i};
	}

	void AddEdge(const vertex_type& from, const vertex_type& to) {
		edges[from].insert(to);
		edges[to].insert(from);
	}

	vector<vector<vertex_type>> Components() {
		vector<vector<vertex_type>> result;
		map<vertex_type, vertex_type> visited;
		vertex_type current;

		for (const auto& vt : edges) {
			if (visited.find(vt.first) == visited.end()) {

				current = vt.first;
				vector<vertex_type> p;
				queue<vertex_type> queue;

				queue.push(current);
		
				while (!queue.empty()) {
					current = queue.front();
					queue.pop();

					if (visited.find(current) == visited.end()) {
						visited.insert({current, current});
						p.push_back(current);

						for (const vertex_type& ver : edges[current]) {
							queue.push(ver);
						}
					}
				}

				sort(p.begin(), p.end());
				result.push_back(p);

			}
		}

		sort(result.begin(), result.end(), 
		[](const vector<vertex_type>& v1, const vector<vertex_type> v2) {
			return v1 < v2;
		});

		return result;
	}
};

int main() {

	size_t n = 0, m = 0;
	cin >> n >> m;

	Graph gr(n);

	for (size_t i = 0; i < m; i++) {
		size_t from, to;
		cin >> from >> to;
		gr.AddEdge(from, to);
	}

	std::chrono::time_point<std::chrono::system_clock>  start1 = std::chrono::system_clock::now();

	gr.Components();

	std::chrono::time_point<std::chrono::system_clock>  end1 = std::chrono::system_clock::now();
    

    std::cout << n+m << " " << std::chrono::duration<double>(end1-start1).count() << "\n";

	return 0;
}