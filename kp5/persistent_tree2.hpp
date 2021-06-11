#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include "utilitys.hpp"

using namespace std;

static unsigned long long index = 1;

template<class K, class V>
struct PersistentTree {

	struct node {

		using h_type = unsigned int;
		using nre_type = unsigned int;
		using idx_type = unsigned long long;
		using node_ptr = shared_ptr<node>;

		node_ptr l = nullptr;
		node_ptr r = nullptr;
		K key;
		V value;
		h_type h;
		nre_type nre;
		idx_type idx;

		node(const K& _k, const V& _v): key(_k), value(_v), h(1), nre(0) {
			idx=index; 
			index++;
		}
		node(const K& _k, const V& _v, const h_type& _h, const nre_type& _nre): key(_k), value(_v), h(_h), nre(_nre) {
			idx=index; 
			index++;
		}

		node_ptr RightRotate(node_ptr head) {
			node_ptr temp1 = make_shared<node>(head->l->key, head->l->value, head->l->h, 1 + head->l->nre + head->nre);
			node_ptr temp2 = make_shared<node>(head->key, head->value, head->h, head->nre);
			temp1->l = head->l->l;
			temp1->r = temp2;
			temp2->l = head->l->r;
			temp2->r = head->r;
			FixHeight(temp2);
			FixHeight(temp1);
			return temp1;
		}

		node_ptr LeftRotate(node_ptr head) {

			if (head->key == head->r->key)
				return head;

			node_ptr temp1 = make_shared<node>(head->r->key, head->r->value, head->r->h, head->r->nre);
			node_ptr temp2 = make_shared<node>(head->key, head->value, head->h, head->nre - 1 - head->r->nre);
			temp1->l = temp2;
			temp1->r = head->r->r;
			temp2->l = head->l;
			temp2->r = head->r->l;
			FixHeight(temp2);
			FixHeight(temp1);
			return temp1;
		}

		node_ptr Balancing(node_ptr head) {
			node_ptr temp = make_shared<node>(head->key, head->value, head->h, head->nre);
			temp->l = head->l;
			temp->r = head->r;
			FixHeight(temp);
			if (Balance(temp)==2) {
				if (Balance(temp->l)<0) {
					temp->l = LeftRotate(head->l);
				}
				return RightRotate(temp); 
			} else if (Balance(temp)==-2) {
				if (Balance(temp->r)>0) {
					temp->r = RightRotate(head->r);
				}
				return LeftRotate(temp);
			}
			return temp;
		}

		node_ptr Insert(const node_ptr parent, const K& key, const V& value, const double slab) {
			static unsigned int index = 1;
			if (parent) {
				node_ptr temp;
				if (key.y(slab) > parent->key.y(slab)) {
					temp = make_shared<node>(parent->key, parent->value, parent->h, parent->nre + 1);
					temp->l = parent->l;
					temp->r = Insert(parent->r, key, value, slab);
				} else {
					temp = make_shared<node>(parent->key, parent->value, parent->h, parent->nre);
					temp->r = parent->r;
					temp->l = Insert(parent->l, key, value, slab);
				}

				return Balancing(temp);
			}

			return make_shared<node>(key, value);
		}

		node_ptr MinRight(node_ptr head) {
			node_ptr temp = head;
			while (temp->l) {
				temp = temp->l;
			}
			return temp;
		}

		node_ptr RemoveMin(node_ptr head) {
			if (!head->l) {
				node_ptr temp = head->r;
				return temp;
			}
			node_ptr temp = make_shared<node>(head->key, head->value, head->h, head->nre);
			temp->r = head->r;
			temp->l = RemoveMin(head->l);
			return Balancing(temp);
		}

		node_ptr Remove(node_ptr parent, const K& key, const double slab) {
			if (!parent) 
				return nullptr;

			node_ptr temp;
			if (key == parent->key) {
				if (!parent->l and !parent->r) {
					return nullptr;
				}

				if (!parent->r) {
					return parent->l;
				}

				node_ptr min_right = MinRight(parent->r);
				temp = make_shared<node>(min_right->key, min_right->value, parent->h, parent->nre - 1);
				temp->l = parent->l;
				temp->r = RemoveMin(parent->r);
			} else if (key.y(slab) < parent->key.y(slab)) {
				temp = make_shared<node>(parent->key, parent->value, parent->h, parent->nre);
				temp->r = parent->r;
				temp->l = Remove(parent->l, key, slab);
			} else {
				temp = make_shared<node>(parent->key, parent->value, parent->h, parent->nre - 1);
				temp->l = parent->l;
				temp->r = Remove(parent->r, key, slab);
			}
			
			return Balancing(temp);
		}

		nre_type FindNumberAbove(const node_ptr head, const Point& p, bool *onEdge, long* left_ancestor, long* right_ancestor) {
			if (head) {
				if (p.y > head->key.y(p.x)) {
					*left_ancestor = static_cast<long>(head->value);
					return FindNumberAbove(head->r, p, onEdge, left_ancestor, right_ancestor);
				} else if (p.y == head->key.y(p.x)) {
					*left_ancestor = static_cast<long>(head->value);
					*right_ancestor = static_cast<long>(head->value);
					*onEdge = true;
					return 0;
				} else {
					*right_ancestor = static_cast<long>(head->value);
					return head->nre + 1 + FindNumberAbove(head->l, p, onEdge, left_ancestor, right_ancestor);
				}
			}

			return 0;
		}

		h_type Height(const node_ptr head) const {
			return head ? head->h : 0;
		}

		int Balance(const node_ptr head) const {
			return head ? Height(head->l)-Height(head->r) : 0;
		}

		void FixHeight(node_ptr head) {
			head->h = (Height(head->l)>Height(head->r) ? Height(head->l) : Height(head->r))+1;
		}

		void Print(const node_ptr head, unsigned int tab) const {
			if (head) {
				Print(head->r, tab + 1);
				for (unsigned int i = 0; i < tab; i++) std::cout << "\t";
				std::cout << "(" << head->key << ", " << head->value << ", " << head->h << ", " << head->nre << ", " << head->idx << ")\n";
				Print(head->l, tab + 1);
			}
		}

		friend std::ostream& operator<<(std::ostream& out, const node& p) {
			out << p.key << " " << p.value << " " << p.h << " " << p.nre << " " << p.idx;
			return out;
		}

		friend std::ofstream& operator<<(std::ofstream& out, const node& p) {
			out << p.key << " " << p.value << " " << p.h << " " << p.nre << " " << p.idx;
			return out;
		}

	};

	using node_ptr = shared_ptr<node>;

	unsigned int number_of_versions = 0;
	vector<node_ptr> trees;

	void Insert(const K& key, const V& value, const double slab, const bool flag = false) {
		if (!flag) {
			if (trees.empty()) {
				trees.push_back(nullptr);
				trees[0] = make_shared<node>(key, value);
			} else {
				trees.push_back(nullptr);
				trees[number_of_versions] = trees[number_of_versions]->Insert(trees[number_of_versions - 1], key, value, slab);
			}
			number_of_versions++;
		} else {
			if (trees.empty()) {
				trees.push_back(nullptr);
				trees[0] = make_shared<node>(key, value);
				number_of_versions++;
			} else {
				trees[number_of_versions - 1] = trees[number_of_versions - 1]->Insert(trees[number_of_versions - 1], key, value, slab);
			}
		}
	}

	void Remove(const K& key, const double slab, const bool flag = false) {
		if (!flag) {
			trees.push_back(nullptr);
			trees[number_of_versions] = trees[number_of_versions]->Remove(trees[number_of_versions - 1], key, slab);
			number_of_versions++;
		} else {
			trees[number_of_versions - 1] = trees[number_of_versions - 1]->Remove(trees[number_of_versions - 1], key, slab);
		}
	}

	void NotChange() {
		trees.push_back(nullptr);
		trees[number_of_versions] = trees[number_of_versions - 1];
		number_of_versions++;
	}

	unsigned int FindNumberAbove(const unsigned int version, const Point& p, long* left_ancestor, long* right_ancestor) {
		if (version >= trees.size()) {
			print("bad version: ", version);
			throw "There is no such version";
		}

		bool onEdge = false;
		unsigned int result = trees[version]->FindNumberAbove(trees[version], p, &onEdge, left_ancestor, right_ancestor);

		if (onEdge) 
			result = 1;

		return result; 
	}

	void Print(const unsigned int version) const {
		if (version >= trees.size()) {
			cout << "|--------------------------------------------------|" << endl;
			cout << "There is no such version of tree" << endl;
			cout << "|--------------------------------------------------|" << endl;
			return;
		}

		cout << "|--------------------------------------------------|" << endl;
		cout << "[" << version << "]" << endl;
		trees[version]->Print(trees[version], 0);
		cout << "|--------------------------------------------------|" << endl;
	}

	void Print() const {
		unsigned int v = 1;
		cout << number_of_versions << " versions in total:\n";
		for (node_ptr tree : trees) {
			cout << "|--------------------------------------------------|" << endl;
			cout << "[" << v << "]" << endl;
			tree->Print(tree, 0);
			cout << "|--------------------------------------------------|" << endl;
			v++;
		}
	}

	friend ofstream& operator<<(ofstream& out, PersistentTree& pt) {
		map<unsigned long long, node> visited;

		node_ptr curr;
		out << pt.trees.size() << "\n";

		for (size_t i = 0; i < pt.trees.size(); i++) {
			curr = pt.trees[i];

			if (curr == nullptr) {
				out << "0 0 0 0 0 0 0 0 ";
				continue;
			} else {
				out << *curr << " ";
			}

			queue<node_ptr> visited_in_this_tree;

			if (curr->l != nullptr)
				visited_in_this_tree.push(curr->l);

			if (curr->r != nullptr)
				visited_in_this_tree.push(curr->r);

			while (!visited_in_this_tree.empty()) {
				curr = visited_in_this_tree.front();
				visited_in_this_tree.pop();

				visited.insert({curr->idx, *curr});

				if (curr->l != nullptr)
					visited_in_this_tree.push(curr->l);

				if (curr->r != nullptr)
					visited_in_this_tree.push(curr->r);
			}
		}

		out << "\n" << visited.size() << "\n";

		for (const auto n : visited) {
			out << n.second << " ";
		}

		out << "\n";

		for (size_t i = 0; i < pt.trees.size(); i++) {
			curr = pt.trees[i];

			if (curr == nullptr) 
				continue;

			queue<node_ptr> visited_in_this_tree;
			visited_in_this_tree.push(curr);

			while (!visited_in_this_tree.empty()) {
				curr = visited_in_this_tree.front();
				visited_in_this_tree.pop();

				if (curr->l != nullptr) {
					out << curr->idx << " " << curr->l->idx << " " << "0" << " ";
					visited_in_this_tree.push(curr->l);
				}

				if (curr->r != nullptr) {
					out << curr->idx << " " << curr->r->idx << " " << "1" << " ";
					visited_in_this_tree.push(curr->r);
				}
			}
		}

		return out;
	}


	friend ifstream& operator>>(ifstream& in, PersistentTree& pt) {

		in >> pt.number_of_versions;
		for (size_t i = 0; i < pt.number_of_versions; i++) {
			Edge key;
			unsigned int value;
			unsigned int h;
			unsigned int nre;
			unsigned long long idx;

			in >> key >> value >> h >> nre >> idx;
			node_ptr np = make_shared<PersistentTree<K, V>::node>(key, value, h, nre);
			np->idx = idx;
			if (idx == 0) np = nullptr;
			pt.trees.push_back(np);
		}

		unsigned int number_of_inside_vertexes;
		in >> number_of_inside_vertexes;

		vector<node_ptr> inside_vertexes;

		for (size_t i = 0; i < number_of_inside_vertexes; i++) {
			Edge key;
			unsigned int value;
			unsigned int h;
			unsigned int nre;
			unsigned long long idx;

			in >> key >> value >> h >> nre >> idx;
			node_ptr np = make_shared<PersistentTree<K, V>::node>(key, value, h, nre);
			np->idx = idx;
			inside_vertexes.push_back(np);
		}

		unsigned long long num_from, num_to, side;
		while (in >> num_from >> num_to >> side) {
			node_ptr from = nullptr;
			node_ptr to = nullptr;

			for (size_t i = 0; i < inside_vertexes.size(); i++) {
				if (inside_vertexes[i]->idx == num_from) {
					from = inside_vertexes[i];
				}

				if (inside_vertexes[i]->idx == num_to) {
					to = inside_vertexes[i];
				}
			}

			if (from == nullptr) {
				for (size_t i = 0; i < pt.trees.size(); i++) {
					if (pt.trees[i] == nullptr) 
						continue;
					
					if (pt.trees[i]->idx == num_from) {
						from = pt.trees[i];
					}
				}
			}

			if (side == 0) {
				from->l = to;
			} else if (side == 1) {
				from->r = to;
			}

		}

		return in;
	}

};