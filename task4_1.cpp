#include <iostream>
#include <stack>
#include <functional>




template <typename T = int, typename comparator = std::less<T>>
class BinaryTree {
    struct Node {
        Node(const T& data): data(data), left(nullptr), right(nullptr), height(1) {};

        T data;
        Node *left;
        Node *right;
        size_t height;
    };
private:
    Node *root;
    void fixHeight(Node *node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixHeight(node);
        fixHeight(tmp);

        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixHeight(node);
        fixHeight(tmp);

        return tmp;
    }

    Node* doBalance(Node *node) {
        fixHeight(node);

        switch (getBalance(node)) {
            case 2:
                if (getBalance(node->right) < 0) {
                    node->right = rotateRight(node->right);
                    return rotateLeft(node);
                }
                break;
            case -2:
                if (getBalance(node->left) > 0) {
                    node->left = rotateLeft(node->left);
                    return rotateRight(node);
                }
                break;
            default:
                return node;
        }
    }

    /*Node* findAndRemoveMin(Node *node) {
        while (node->left) {
            node = node->left;
        }
        if (!node->left) {
            return node->right;
        }

        return doBalance(node);
    }*/

    Node* addInternal(Node *node, const T& data) {
        if (!node) {
            return new Node(data);
        }
        if (cmp(node->data, data) || node->data == data) {
            node->right = addInternal(node->right, data);
        } else {
            node->left = addInternal(node->left, data);
        }

        return doBalance(node);
    }

    Node* removeInternal(Node *node, const T& data) {
        if (!node) {
            return nullptr;
        }
        if (cmp(node->data, data)) {
            node->right = removeInternal(node->right, data);
        } else if (!cmp(node->data, data) && node->data != data) {
            node->left = removeInternal(node->left, data);
        } else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right) {
                return left;
            }

            Node *min = findAndRemoveMin(right);
            min->left = left;

            return doBalance(min);
        }
        return doBalance(node);
    }

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinaryTree(): root(nullptr) {};
    ~BinaryTree() {
        destroyTree(root);
    }

    comparator cmp;

    bool Has(const T& data);
    void Add(const T& data);
    void Remove(const T& data);

    void preOrder();
};

// ----------------------------------------------------------------------------
template<typename T, typename comparator>
bool BinaryTree<T, comparator>::Has(const T &data) {
    Node *tmp = root;
    while (tmp) {
        if (tmp->data == data) {
            return true;
        } else if (cmp(tmp->data, data)) {
            tmp = tmp->right;
        } else {
            tmp = tmp->left;
        }
    }
    return false;
}

template<typename T, typename comparator>
void BinaryTree<T, comparator>::Add(const T &data) {
    root = addInternal(root, data);
}

template<typename T, typename comparator>
void BinaryTree<T, comparator>::Remove(const T &data) {
    root = removeInternal(root, data);
}

// Прямой обход: корень - левое поддерево - правое поддерево
template<typename T, typename comparator>
void BinaryTree<T, comparator>::preOrder() {
    std::stack<Node*> stack;
    std::stack<Node*> output;

    stack.push(root);
    Node *node = nullptr;

    while (!stack.empty()) {
        node = stack.top();
        stack.pop();
        output.push(node);

        // сначала обходим левое поддерево
        if (node->left != nullptr) {
            stack.push(node->left);
        }
        // потом правое
        if (node->right != nullptr) {
            stack.push(node->right);
        }
    }

    while (!output.empty()) {
        std::cout << output.top()->data << " ";
        output.pop();
    }
}

// ----------------------------------------------------------------------------

int main() {
    BinaryTree<> avlTree;

    int value;
    int n = 0;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> value;
        avlTree.Add(value);
    }

    avlTree.preOrder();

    return 0;
}








