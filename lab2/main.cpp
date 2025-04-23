#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <cctype>
#include <random>
#include <ctime>
#include <utility>
#include <chrono>
using namespace std;


class BST {
    struct Node {
        string key;
        uint64_t value;
        int priority;
    
        Node* left;
        Node* right;
        
        Node(string key, uint64_t value, int p)
            : key(key), value(value), left(nullptr), right(nullptr), priority(p) {}
    };

    Node* root; 

    Node* find(Node *node, const string& key) {
        while (node) {
            if (key < node->key) {
                node = node->left;
            }
            else if (key > node->key) {
                node = node->right;
            }
            else {
                return node;
            }
        }

        return nullptr;
    }

    Node* insert(Node* node, const string& key, const uint64_t& value) {
        if (node == nullptr) {
            return new Node(key, value, genPriority());
        }

        if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else if (key > node->key) {
            node->right = insert(node->right, key, value);
        }

        return node;
    }

    static int genPriority() {
        static mt19937_64 rng{static_cast<uint64_t>(chrono::high_resolution_clock::now().time_since_epoch().count())};
        static uniform_int_distribution<int> dist(numeric_limits<int>::min(), numeric_limits<int>::max());
        return dist(rng);
    }

    Node* findMax(Node* node) {
        while (node != nullptr && node->right != nullptr) {
            node = node->right;
        }

        return node;
    }

    Node* remove(Node* t, const std::string& key) {
        if (!t) return nullptr;
        if (key < t->key) {
            t->left = remove(t->left, key);
        } else if (t->key < key) {
            t->right = remove(t->right, key);
        } else {
            Node* tmp = t;
            t = merge(t->left, t->right);
            delete tmp;
        }
        return t;
    }

    void removeTree(Node *node) {
        if (node == nullptr) {
            return;
        }

        removeTree(node->left);
        removeTree(node->right);

        delete node;
    }

    pair<Node*, Node*> split(Node* t, const string& key) {
        if (t == nullptr) {
            return pair<Node*, Node*>(nullptr, nullptr);
        }

        if (key < t->key) {
            auto splitted = split(t->left, key);
            auto t1 = splitted.first;
            auto t2 = splitted.second;

            t->left = t2;

            return pair<Node*, Node*>(t1, t);
        } else {
            auto splitted = split(t->right, key);
            auto t1 = splitted.first; 
            auto t2 = splitted.second;

            t->right = t1; 

            return pair<Node*, Node*>(t, t2);
        }
    }

    Node* merge(Node* t1, Node* t2) {
        if (t1 == nullptr) {
            return t2;
        }

        if (t2 == nullptr) {
            return t1;
        }

        if (t1->priority >= t2->priority) {
            t1->right = merge(t1->right, t2);
            return t1;
        }

        t2->left = merge(t1, t2->left);
        return t2; 
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        removeTree(root);
    }

    bool search(const string& key, uint64_t& value) {
        Node* res = find(root, key);

        if (res == nullptr) {
            return false;
        } 

        value = res->value;
        return true;
    }

    bool insert(const string& key, const uint64_t& value) {
        uint64_t tmp;
        if (search(key, tmp)) {
            return false;
        }

        auto splitted = split(root, key);
        auto t1 = splitted.first;
        auto t2 = splitted.second;

        t1 = merge(t1, new Node(key, value, genPriority()));
        root = merge(t1, t2);

        return true;
    }

    bool eraseNode(const string& key) {
        uint64_t tmp; 

        if (!search(key, tmp)) return false;
        root = remove(root, key); 

        return true; 
    }
  
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ifstream file("input.txt");

    BST bst;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;

        char cmd = line[0];

        if (cmd == '+') {
            istringstream iss(line.substr(1));
            string key;
            uint64_t value; 

            iss >> key >> value; 
            transform(key.begin(), key.end(), key.begin(), 
                [](unsigned char c) {return tolower(c); });

            if (bst.insert(key, value)) {
                cout << "OK\n";
            } else {
                cout << "Exist\n";
            }
        } else if (cmd == '-') {
            string key = line.substr(2);
            transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c) { return tolower(c); });

            if (bst.eraseNode(key)) {
                cout << "OK\n";
            } else {
                cout << "NoSuchWord\n";
            }
        } else {
            string key = line; 
            transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c) { return tolower(c); });

            uint64_t value; 

            if (bst.search(key, value)) {
                cout << "OK: " << value << "\n";
            } else {
                cout << "NoSuchWord\n";
            }
        }
    }

    return 0;
}