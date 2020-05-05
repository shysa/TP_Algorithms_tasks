#include <iostream>
#include <stack>

template <typename T = int, typename comparator = std::less_equal<T>>
class AVLTree {
    struct Node {
        Node(const T& data): data(data), left(nullptr), right(nullptr) {};

        T data;
        Node *left;
        Node *right;
    };
private:
    Node *root;

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void addInternal(Node* &node, const T& data) {
        if (!node) {
            node = new Node(data);
            return;
        }
        // root->key <= K - в правое поддерево
        // иначе в левое
        if ( cmp(node->data, data) ) {
            addInternal(node->right, data);
        } else {
            addInternal(node->left, data);
        }
    }

public:
    AVLTree(): root(nullptr) {};
    ~AVLTree() {
        destroyTree(root);
    }

    comparator cmp;

    void Add(const T& data);
    void preOrder();
};


template<typename T, typename comparator>
void AVLTree<T, comparator>::Add(const T &data) {
    addInternal(root, data);
}

// Прямой обход: корень - левое поддерево - правое поддерево
template<typename T, typename comparator>
void AVLTree<T, comparator>::preOrder() {
    std::stack<Node*> stack;
    std::stack<Node*> output;

    stack.push(root);
    Node *node = nullptr;

    while (!stack.empty()) {
        node = stack.top();
        stack.pop();
        output.push(node);
        std::cout << node->data << " ";

        // сначала обходим левое поддерево, потом правое
        // => в стек заносим правое поддерево сначала, чтобы левое обрабатывалось раньше
        if (node->right != nullptr) {
            stack.push(node->right);
        }
        if (node->left != nullptr) {
            stack.push(node->left);
        }

    }
}

// ----------------------------------------------------------------------------

int main() {
    AVLTree<> binaryTree;

    int value = -1;
    int n = 0;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> value;
        binaryTree.Add(value);
    }

    binaryTree.preOrder();

    return 0;
}








