#include <iostream>
#include <stack>

// ------------------------ BINARY TREE ---------------------------------------
template <typename T = int, typename comparator = std::less_equal<T>>
class BinaryTree {
    struct Node {
        Node(const T& data): data(data), left(nullptr), right(nullptr), height(1) {};

        T data;
        Node *left;
        Node *right;
        int height;
    };
private:
    Node *root;

    void destroyTree(Node *node) {
        if (!node) {
            return;
        } else {
            std::stack<Node*> nodes;
            nodes.push(node);

            while (!nodes.empty()) {
                node = nodes.top();
                nodes.pop();

                if (node->right != nullptr) {
                    nodes.push(node->right);
                }
                if (node->left != nullptr) {
                    nodes.push(node->left);
                }

                delete node;
            }
        }
    }

    void addInternal(Node* node, const T& data) {
        Node *tmp = new Node(data);

        if (!node) {
            root = tmp;
            return;
        }

        // root->key <= K - в правое поддерево
        // иначе в левое
        // ищем нужное место, заодно высчитывая высоту
        while ( (cmp(node->data, tmp->data) && node->right) || (!cmp(node->data, tmp->data) && node->left) ) {
            node = cmp(node->data, tmp->data) ? node->right : node->left;
            (tmp->height)++;
        }

        (tmp->height)++;
        if ( cmp(node->data, data) ) {
            node->right = tmp;
        } else {
            node->left = tmp;
        }
    }

    int getDepthInternal(BinaryTree::Node *node) {
        int maxHeight = 0;

        std::stack<Node*> nodes;

        if (!node) {
            return 0;
        } else {
            nodes.push(node);

            while (!nodes.empty()) {
                node = nodes.top();
                nodes.pop();

                if (node->right != nullptr) {
                    nodes.push(node->right);
                }
                if (node->left != nullptr) {
                    nodes.push(node->left);
                }

                if (maxHeight < node->height) {
                    maxHeight = node->height;
                }
            }
        }

        return maxHeight;
    }


public:
    BinaryTree(): root(nullptr) {};
    ~BinaryTree() {
        destroyTree(root);
    }

    comparator cmp;

    void Add(const T& data);
    int getDepth();
};

template<typename T, typename comparator>
void BinaryTree<T, comparator>::Add(const T &data) {
    addInternal(root, data);
}

template<typename T, typename comparator>
int BinaryTree<T, comparator>::getDepth() {
    return getDepthInternal(root);
}
// ----------------------------------------------------------------------------


// ------------------------ DECART TREE ---------------------------------------
template <typename T = int, typename comparator = std::less<T>>
class DecartTree {
    struct Node {
        Node(const T& data, const int& priority): data(data), priority(priority), left(nullptr), right(nullptr) {};

        T data;
        int priority;
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

    // Разделяет найденное дерево узла node на два поддерева left и right, где:
    // left = все элементы < data,
    // right = все элементы >= data.
    void split(Node* node, const int& data, Node* &left, Node* &right) {
        if (!node) {
            left = nullptr;
            right = nullptr;
        } else {
            if (data < node->data) {
                split(node->left, data, left, node->left);
                right = node;
            } else {
                split(node->right, data, node->right, right);
                left = node;
            }
        }
    }

    void addInternal(Node* &node, Node* tmp) {
        if (!node) {
            node = tmp;
            return;
        }

        if (node->priority < tmp->priority) {
            // если нашли нужный узел с меньшим приоритетом, чем у добавляемого элемента,
            // то разрезаем дерево на два поддерева, которые станут l и r потомками добавляемого узла tmp
            split(node, tmp->data, tmp->left, tmp->right);
            node = tmp;
        } else {
            // спускаемся по дереву дальше
            if ( cmp(tmp->data, node->data) ) {
                addInternal(node->left, tmp);
            } else {
                addInternal(node->right, tmp);
            }
        }
    }

    int getDepthInternal(DecartTree::Node *node) {
        if (!node) {
            return 0;
        } else {
            int depthLeft = getDepthInternal(node->left);
            int depthRight = getDepthInternal(node->right);

            return 1 + std::max(depthLeft, depthRight);
        }
    }

public:
    DecartTree(): root(nullptr) {};
    ~DecartTree() {
        destroyTree(root);
    }

    comparator cmp;

    void Add(const T& data, const int& priority);
    int getDepth();
};

template<typename T, typename comparator>
void DecartTree<T, comparator>::Add(const T &data, const int& priority) {
    Node *tmp = new Node(data, priority);
    addInternal(root, tmp);
}

template<typename T, typename comparator>
int DecartTree<T, comparator>::getDepth() {
    return getDepthInternal(root);
}
// ----------------------------------------------------------------------------


int main() {
    BinaryTree<> binaryTree;
    DecartTree<> decartTree;

    int value = 0;
    int priority = 0;
    int N = 0;

    std::cin >> N;

    for (int i = 0; i < N; i++) {
        std::cin >> value >> priority;
        binaryTree.Add(value);
        decartTree.Add(value, priority);
    }

    std::cout << std::abs(binaryTree.getDepth() - decartTree.getDepth());


    return 0;
}

