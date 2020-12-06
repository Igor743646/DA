#include <iostream>
#include <chrono>
#include <fstream>

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

struct TAVLTree {
    TAVLTree *left = 0;
    TAVLTree *right = 0;

    NMystd::TString key;
    unsigned long long value;
    unsigned int h;

    TAVLTree () = default;
    TAVLTree (const NMystd::TString& k, const unsigned long long v);
    ~TAVLTree () = default;
    void RemoveTree(TAVLTree *head);

    TAVLTree* RightRotate(TAVLTree *head);
    TAVLTree* LeftRotate(TAVLTree *head);
    TAVLTree* Balancing(TAVLTree *head);
    TAVLTree* MinRight(TAVLTree* head);
    TAVLTree* RemoveMin(TAVLTree* head);

    TAVLTree* Insert(TAVLTree *head, const NMystd::TString& key, const unsigned long long *value);
    TAVLTree* Remove(TAVLTree *head, const NMystd::TString& key);
    bool Search(TAVLTree *head, const NMystd::TString& key);

    void Print(const TAVLTree *head, unsigned int n);
    void Save(const TAVLTree *head, FILE *s);
    TAVLTree* BinInsert(TAVLTree *head, const NMystd::TString& key, const unsigned long long value);
    TAVLTree* Load(FILE *s);

    unsigned int Height(const TAVLTree *head);
    int Balance(const TAVLTree *head);
    void FixHeight(TAVLTree *head);
};

TAVLTree::TAVLTree (const NMystd::TString& k, const unsigned long long v): key(k), value(v), h(1) {} 

void TAVLTree::RemoveTree(TAVLTree *head){
    if (!head) return;
    if (head->right) {
        RemoveTree(head->right);
    }
    if (head->left) {
        RemoveTree(head->left);
    }
    delete head;
} 

TAVLTree* TAVLTree::RightRotate(TAVLTree *head) {
    TAVLTree *temp = head->left;
    head->left = temp->right;
    temp->right = head;
    FixHeight(head);
    FixHeight(temp);
    return temp;
}

TAVLTree* TAVLTree::LeftRotate(TAVLTree *head) {
    TAVLTree *temp = head->right;
    head->right = temp->left;
    temp->left = head;
    FixHeight(head);
    FixHeight(temp);
    return temp;
}

TAVLTree* TAVLTree::Balancing(TAVLTree *head) {
    FixHeight(head);
    if (Balance(head)==2) {
        if (Balance(head->left)<0) {
            head->left = LeftRotate(head->left);
        }
        return RightRotate(head); 
    } else if (Balance(head)==-2) {
        if (Balance(head->right)>0) {
            head->right = RightRotate(head->right);
        }
        return LeftRotate(head);
    }
    return head;
}

TAVLTree* TAVLTree::MinRight(TAVLTree* head) {
    TAVLTree *temp = head;
    while (temp->left) {
        temp = temp->left;
    }
    return temp;
}

TAVLTree* TAVLTree::RemoveMin(TAVLTree* head) {
    if (!head->left) {
        TAVLTree *temp = head->right;
        delete head;
        return temp;
    }
    head->left = RemoveMin(head->left);
    return Balancing(head);
}

TAVLTree* TAVLTree::Insert(TAVLTree *head, const NMystd::TString& key, const unsigned long long *value) {
    if (head) {
        if (key == head->key) {
            std::cout << "Exist\n";
            return head;
        } else if (key < head->key) {
            head->left = Insert(head->left, key, value);
        } else {
            head->right = Insert(head->right, key, value);
        }
        return Balancing(head);
    }
    std::cout << "OK\n";
    return new TAVLTree(key, *value);
}

TAVLTree* TAVLTree::Remove(TAVLTree *head, const NMystd::TString& key) {
    if (!head) {
        std::cout << "NoSuchWord\n";
        return 0;
    } else if (key == head->key) {
        if (!head->left && !head->right) {
            std::cout << "OK\n";
            delete head;
            return 0;
        }
        if (!head->right) {
            TAVLTree *temp = head->left;
            delete head;
            std::cout << "OK\n";
            return temp;
        }

        head->key = MinRight(head->right)->key;
        head->value = MinRight(head->right)->value;

        head->right = RemoveMin(head->right);
        std::cout << "OK\n";
    } else if (key < head->key) {
        head->left = Remove(head->left, key);
    } else {
        head->right = Remove(head->right, key);
    }
    
    return Balancing(head);
}

bool TAVLTree::Search (TAVLTree *head, const NMystd::TString& key) {
    TAVLTree *temp = head;
    while (temp) {
        if (key==temp->key) {
            std::cout << "OK: " << temp->value << "\n";
            return true;
        } else if (key > temp->key) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }

    std::cout << "NoSuchWord\n";
    return false;
}

void TAVLTree::Print(const TAVLTree *head, unsigned int n) {
    if (head) {
        Print(head->right, n+1);
        for (unsigned int i=0; i<n; i++) std::cout << "\t";
        std::cout << "(" << head->key << ", " << head->value << ")\n";
        Print(head->left, n+1);
    }
}

void TAVLTree::Save(const TAVLTree *head, FILE *s) {
    if (head) {
        fprintf(s, "%s %llu ", head->key.str, head->value);
        Save(head->left, s);
        Save(head->right, s);
    }
}

TAVLTree* TAVLTree::BinInsert(TAVLTree *head, const NMystd::TString& key, const unsigned long long value) {
    if (head) {
        if (key > head->key) {
            head->right = BinInsert(head->right, key, value);
        } else {
            head->left = BinInsert(head->left, key, value);
        } 
        return head;
    }
    return new TAVLTree(key, value);
}

TAVLTree* TAVLTree::Load(FILE *s) {
    TAVLTree *res = 0; 
    char str[256];
    unsigned long long val;
    while (fscanf(s, "%s %llu ", str, &val)>0) {
        res = res->BinInsert(res, NMystd::TString(str), val);
    }
    return res;
}

unsigned int TAVLTree::Height(const TAVLTree *head) {
    return head ? head->h : 0;
}

int TAVLTree::Balance(const TAVLTree *head) {
    return head ? Height(head->left)-Height(head->right) : 0;
}

void TAVLTree::FixHeight(TAVLTree *head) {
    head->h = (Height(head->left)>Height(head->right) ? Height(head->left) : Height(head->right))+1;
}

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
            } else {
                std::cin >> c;
                FILE *op = fopen(c.str, "r");
                if (op == nullptr) {
                    std::cout << "ERROR: can not open file\n";
                } else {
                    tree->RemoveTree(tree);
                    tree = tree->Load(op);
                    fclose(op);
                    std::cout << "OK\n";
                    
                }
            }
        } else {
            ToLower(&c);
            tree->Search(tree, c);
        }
    }
    tree->RemoveTree(tree);
    return 0;
}