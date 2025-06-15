
#include <vector>
#include <cassert>
#include <iostream>
#include <functional>
#include "median.cc"
using namespace std;


template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

istream& operator>>(istream& is, vector<int>& vec) {
    string x;
    vec = vector<int>();
    while (is >> x && x != ".") {
        vec.push_back(stoi(x));
    }
    return is;
}


template <typename T>
class node {
    T val;
    node<T> *left;
    node<T> *right;
    int discr;
    int dim;
    int depth;
    node(int depth) : depth(depth), left(NULL), right(NULL) {
        k = depth % dim;
    }
    node(T val, int depth) : val(val), depth(depth), left(NULL), right(NULL) {
        dim = val.size();
        k = depth % dim;
    }
    ~node() {
        delete left;
        delete right;
    }
    void print(string prefix_root="", string prefix_left="", string prefix_right="") {
        string bar_space = "|" + string(dim*4-1, ' ');
        string space = string(dim*4, ' ');
        string line = string(dim*4 - 2, '-');
        if (left != NULL) left->print(prefix_left + " ." + line, prefix_left + space, prefix_left + bar_space);
        cout << prefix_root << val;
        cout << endl;
        if (right != NULL) right->print(prefix_right + " '" + line, prefix_right + bar_space, prefix_right + space);
    }
};


template <typename T>
class kdtree {

    node<T> *root;
    int dim;

    node<T>* insert_list(node<T> *p, const vector<vector<T>> &v) {
        if (v.size() == 0) {
            return NULL;
        }
        int k = p->k;
        vector<T> keys(v.size());
        for (int i = 0; i < v.size(); i++) {
            keys[i] = v[i][k];
        }
        quickselect(keys, keys.size()/2);
        
    }


    void insert(node<T> *p, const vector<T> &val) {
        if (val < p) {
            if (p->left == NULL) {
                p->left = new node<T>(val, p->depth+1);
            }
            else {
                insert(p->left, val);
            }
        }
        else {
            if (p->right == NULL) {
                p->right = new node<T>(val, p->depth+1);
            }
            else {
                insert(p->right, val);
            }
        }
    }

public:
    kdtree(int dim) : root(NULL), dim(dim) {
    }
    ~kdtree() {
        delete root;
    }
    kdtree(const vector<vector<T>> &v) : root(NULL), dim(v[0].size()) {
        root = insert_list(v);
    }
    kdtree(const kdtree &t) {
        this = t;
    }
    kdtree<T> &operator=(const kdtree &t) {
        return *this;
    }

    void insert(const vector<T> &val) {
        assert(val.size() == dim);
        if (root == NULL) {
            root = new node<T>(val, 0);
        }
        else {
            insert(root, val);
        }
    }

    vector<T> as_vector() {
        vector<T> v;
        if (root == NULL) return v;
        vector<node<T>*> stack;
        stack.push_back(root);
        while (!stack.empty()) {
            node<T> *p = stack.back();
            stack.pop_back();
            v.push_back(p->val);
            if (p->left != NULL) stack.push_back(p->left);
            if (p->right != NULL) stack.push_back(p->right);
        }
        return v;
    }

    void calculate_depth(node<T>* p, int depth, int& max_depth) {
        if (p == NULL) return;
        if (depth > max_depth) max_depth = depth;
        calculate_depth(p->left, depth+1, max_depth);
        calculate_depth(p->right, depth+1, max_depth);
    };

    int getDepth() {
        int max_depth = 0;
        calculate_depth(root, 1, max_depth);
        return max_depth;
    }

    void print() {
        cout << "kdtree" << endl;
        int depth = getDepth();
        cout << depth << endl;
        for (int i = 0; i < depth; i++) {
            cout << "(" << i%dim << ")" << string(dim*4-3, ' ');
        }
        cout << endl;

        if (root == NULL) return;
        root->print();
    }

};

int main(int argc, char *argv[]) {
    vector<vector<int>> V(0);
    vector<int> v;
    while (cin >> v && v.size() > 0) {
        V.push_back(v);
        cout << "v: " << v << endl;
    }
    for (int i = 0; i < V.size(); ++i) {
        cout << "V[" << i << "]: " << V[i] << endl;
    }
    kdtree<int> t(V);
    t.print();
    return 0;
}   

