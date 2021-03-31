#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>

#include <chrono>

namespace Tree {

    class SufTree {

    public:

        class Node {
            public:
                long l, r, index;
                long suf_link = 0;
                long str_index;
                std::unordered_map<char, long> next_vertexes;

                Node(const long _l = -1, const long _r = -1, const long _index = -1, 
                const long _str_index = -1): l(_l), r(_r), index(_index), 
                str_index(_str_index) {}

                long Length(const long end) const {
                    return std::min(r, end) - l;
                }

        };

        class Iterator {
            public:
                Node* active_node;
                long active_edge_index;
                long active_length;
                unsigned long remainder; // счетчик дополнительных вставок
                char active_edge_char;

                Iterator(Node* _active_node, long _active_edge_index,
                char _active_edge_char, long _active_length, long _remainder): 
                active_node(_active_node), active_edge_index(_active_edge_index), 
                active_length(_active_length), remainder(_remainder), 
                active_edge_char(_active_edge_char) {}

                void CreateEdge(const unsigned long node_count) {
                    active_node->next_vertexes[active_edge_char] = node_count;
                }

                void FirstRule(const std::string* text) {
                    /*  
                        Вершину оставляем корнем. 
                        Активное ребро становится следующим символом нового суффикса. 
                        Активная длина уменьшается на 1. 
                    */
                    active_edge_index++;
                    if (active_edge_index < text->size())
                        active_edge_char = (*text)[active_edge_index];
                    active_length--;
                }

                void ThirdRule(const Node* vertexes) {
                    /*
                        Переходим по суффиксной ссылке, не меняя 
                        активные ребро и длину.
                    */
                    active_node = (Node*) vertexes + active_node->suf_link;
                }

        };

        long end = 0;
        const std::string* text;
        Node* vertexes;
        unsigned long node_count = 1;

        SufTree(const std::string* t): text(t) {

            long si = 1;
            vertexes = new Node[2 * t->size() + 2];

            // создание корня дерева
            vertexes[0] = Node(-1, -1, 0);
            
            // итератор 
            Iterator iter(&vertexes[0], -1, '\0', 0, 0);

            for (size_t i = 0; i < text->size(); i++) {
                end++;
                char char_to_insert = (*text)[i];
                iter.remainder++;

                if (iter.remainder == 1) {
                    iter.active_edge_index = i;
                    iter.active_edge_char = char_to_insert;
                    if (iter.active_length < 0) {
                        iter.active_length = 0;
                    }
                }

                long split_node_index = -1;
                while (iter.remainder > 0) {


                    if (iter.active_node->next_vertexes.find(iter.active_edge_char) ==  iter.active_node->next_vertexes.end()) { // Если суффикса нет (даже неявного)
                        vertexes[node_count] = Node(iter.active_edge_index, text->size(), node_count, si); // Создаем лист
                        iter.CreateEdge(node_count); // Создаем ребро к новой вершине

                        if (split_node_index != -1) // Если создается вершина не впервые на данном шаге (Правило 2)
                            /*
                                Вершина, вставленная на предыдущем шаге связывается с
                                новой созданной вершиной.
                            */
                            vertexes[split_node_index].suf_link = iter.active_node->index;

                        split_node_index = node_count;
                        
                        if (iter.active_node->index == 0) { // Если активная вершина корневая (Правило 1)
                            iter.FirstRule(text);
                        } else { // Если активная вершина не корневая (Правило 3)
                            iter.ThirdRule(vertexes);
                        }

                    } else {
                        Node* split_node = &vertexes[iter.active_node->next_vertexes[iter.active_edge_char]];
                        if (iter.active_length >= split_node->Length(end)) { // Если не хватило длины split_node
                            iter.active_node = split_node;
                            iter.active_length -= iter.active_node->Length(end);
                            iter.active_edge_index += iter.active_node->Length(end);
                            iter.active_edge_char = (*text)[iter.active_edge_index];
                            continue;
                        }

                        if ((*text)[split_node->l + iter.active_length] == char_to_insert) { // Если след. символ совпал
                            iter.active_length++;
                            if (split_node_index != -1) // (Правило 2)
                                vertexes[split_node_index].suf_link = iter.active_node->index;
                            break;
                        }

                        vertexes[node_count] = Node(split_node->l, split_node->l + iter.active_length, node_count);
                        vertexes[node_count + 1] = Node(i, text->size(), node_count + 1, si);

                        split_node->l += iter.active_length;
                        iter.CreateEdge(node_count);

                        Node* mid_node = &vertexes[node_count];
                        mid_node->next_vertexes[char_to_insert] = node_count + 1;
                        mid_node->next_vertexes[(*text)[split_node->l]] = split_node->index;

                        if (split_node_index != -1) // (Правило 2)
                            vertexes[split_node_index].suf_link = mid_node->index;

                        split_node_index = mid_node->index;

                        if (iter.active_node->index == 0) { // (Правило 1)
                            iter.FirstRule(text);
                        } else { // (Правило 3)
                            iter.ThirdRule(vertexes);
                        }

                        node_count++;
                    }

                    si++;
                    node_count++;
                    iter.remainder--;
                }
            }
        }


        friend std::ostream& operator<<(std::ostream& out, const Tree::SufTree& st) {
            for (unsigned long i = 1; i < st.node_count; i++) {
                for (long j = st.vertexes[i].l; j < st.vertexes[i].r; j++) {
                    out << st.text[j];
                }

                if (st.vertexes[i].str_index != -1)
                    out << " " << st.vertexes[i].str_index;
                out << std::endl;
            }
            return out << std::endl;

        }

        void FindIndexes(const Node* cur, std::vector<long>* r) const {
            if (cur->str_index != -1) {
                r->push_back(cur->str_index);
                return;
            } 

            for (const auto& s : cur->next_vertexes) {
                    FindIndexes(&vertexes[s.second], r);
            }
        }

        bool Find(const std::string* pattern, std::vector<long>* r) {

            if (pattern->size() > text->size())
                return false;

            Node* current = &vertexes[0];
            size_t pt_index = 0;
            long edge_index = current->l;
            
            while (pt_index < pattern->size()) {
                if (edge_index == current->r) {
                    if (current->next_vertexes.find((*pattern)[pt_index]) == current->next_vertexes.end())
                        return false;
                    current = &vertexes[current->next_vertexes[(*pattern)[pt_index]]];
                    edge_index = current->l;
                }

                if ((*pattern)[pt_index] != (*text)[edge_index])
                    return false;

                pt_index++;
                edge_index++;
            }

            FindIndexes(current, r);
            return true;
        }

        ~SufTree() {
            delete [] vertexes;
        }

    };
}



int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::string text;
    std::cin >> text;
    text += '$';
    std::vector<long> occurrences;
    
    Tree::SufTree st(&text);

    

    /*--------------------------------------------*/

    std::string pattern;
    unsigned long pattern_index = 1;

    std::chrono::time_point<std::chrono::system_clock>  start1 = std::chrono::system_clock::now();
    while (std::cin >> pattern) {
        st.Find(&pattern, &occurrences);

        if (occurrences.size() == 0) {
            pattern_index++;
            continue;
        }

        std::stable_sort(occurrences.begin(), occurrences.end());

        printf("%ld: ", pattern_index);
        for (size_t j = 0; j < occurrences.size() - 1; ++j) {
           printf("%ld, ", occurrences[j]);
        }
        
        printf("%ld\n", occurrences.back());

        pattern_index++;
        occurrences.clear();
    }
    std::chrono::time_point<std::chrono::system_clock>  end1 = std::chrono::system_clock::now();
    std::cout << pattern.size() << " " << std::chrono::duration<double>(end1-start1).count() << "\n";

    return 0;
}