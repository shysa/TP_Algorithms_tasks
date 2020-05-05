#include <iostream>
#include <stack>
#include <functional>

// ----------------- AVL TREE ---------------------------------------------------------------
template <typename T, typename comparator = std::less<T>>
class AVLTree {
    struct Node {
        Node(const T& data): data(data), left(nullptr), right(nullptr), height(1), nodes(1) {};

        T data;
        int nodes; // кол-во узлов
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

    // ----------------------------------------------------------------------------
    int getNodes(Node *node) {
        return node ? node->nodes : 0;
    }

    void fixNodes(Node *node) {
        node->nodes = getNodes(node->left) + getNodes(node->right) + 1;
    }
    // ----------------------------------------------------------------------------

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixNodes(node);
        fixNodes(tmp);

        fixHeight(node);
        fixHeight(tmp);

        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixNodes(node);
        fixNodes(tmp);

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
        node->left = findAndRemoveMin(node->left);
        return doBalance(node);
    }*/

    Node* findMin(Node *node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* removeMin(Node *node) {
        if (!node->left) {
            return node->right;
        }
        node->left = removeMin(node->left);
        node->nodes--;

        return doBalance(node);
    }

    Node* addInternal(Node *node, const T& data, int& position) {
        if (!node) {
            return new Node(data);
        }

        node->nodes++;

        if (cmp(node->data, data) || node->data == data) {
            position += getNodes(node->right) + 1;
            node->right = addInternal(node->right, data, position);
        } else {
            node->left = addInternal(node->left, data, position);
        }

        return doBalance(node);
    }

    Node* removeInternal(Node *node, const T& data, int& position) {
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


            //Node *min = findAndRemoveMin(right);
            Node *min = findMin(right);
            min->right = removeMin(right);
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
    AVLTree(): root(nullptr) {};
    ~AVLTree() {
        destroyTree(root);
    }

    comparator cmp;

    bool Has(const T& data);
    void Add(const T& data, int& position);
    void Remove(const T& data, int& position);
};

// ----------------------------------------------------------------------------
template<typename T, typename comparator>
bool AVLTree<T, comparator>::Has(const T &data) {
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
void AVLTree<T, comparator>::Add(const T &data, int& position) {
    root = addInternal(root, data, position);
}

template<typename T, typename comparator>
void AVLTree<T, comparator>::Remove(const T &data, int& position) {
    root = removeInternal(root, data, position);
}


// ----------------------------------------------------------------------------

int main() {
    AVLTree<int> avlTree;

    int command;
    int value;
    int n = 0;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> command >> value;
        switch (command) {
            case 1: {
                avlTree.Add(value);
                break;
            }
            case 2: {
                avlTree.Remove(value);
                break;
            }
            default:
                break;
        }
    }

    return 0;
}








