#include <iostream>
#include <stack>
#include <functional>
#include <vector>

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

    Node* addInternal(Node *node, const T& data, int& position) {
        if (!node) {
            return new Node(data);
        }

        (node->nodes)++;

        if (cmp(data, node->data)) {
            // итоговая позиция точно не меньше чем nodes(right) + 1
            position += getNodes(node->right) + 1;
            node->left = addInternal(node->left, data, position);
        } else {
            node->right = addInternal(node->right, data, position);
        }

        return doBalance(node);
    }

    // =============================================================================================
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
        (node->nodes)--;

        return doBalance(node);
    }

    Node* findMax(Node *node) {
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    Node* removeMax(Node *node) {
        if (!node->right) {
            return node->left;
        }
        node->right = removeMax(node->right);
        (node->nodes)--;

        return doBalance(node);
    }
    // =============================================================================================


    Node* removeInternal(Node *node, const int& position) {
        if (!node) {
            return nullptr;
        }

        // ввели несуществующую позицию
        if (position >= node->nodes) {
            return node;
        }

        int current = 0;

        while (true) {
            int nodes_right = getNodes(node->right);

            // выбираем где искать позицию, в правом или левом поддереве
            if (position - current > nodes_right) {
                node = node->left;
                // ставим текущую позицию для пересчета искомой position
                current += nodes_right + 1;
            }
            else if (position - current < nodes_right) {
                if (node->right) {
                    node = node->right;
                } else {
                    break;
                }
            } else {
                // нашли нужную позицию для удаления
                Node *left = node->left;
                Node *right = node->right;
                int key = node->data;

                delete node;

                if (!right) {
                    return left;
                } else {
                    // иначе если есть оба потомка, то выбираем, на какой элемент заменить
                    if (getHeight(right) > getHeight(left)) {
                        // если правое поддерево глубже, то заменяем на минимальный из правого поддерева
                        Node *min = findMin(right);
                        min->right = removeMin(right);
                        min->left = left;

                        fixNodes(min);
                        return doBalance(min);
                    } else {
                        // иначе заменяем на максимальный из левого
                        Node *max = findMax(left);
                        max->left = removeMax(left);
                        max->right = right;

                        fixNodes(max);
                        return doBalance(max);
                    }
                }

                break;
            }
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

    void Add(const T& data, int& position);
    void Remove(const int& position);
};

// ----------------------------------------------------------------------------
template<typename T, typename comparator>
void AVLTree<T, comparator>::Add(const T &data, int& position) {
    root = addInternal(root, data, position);
}

template<typename T, typename comparator>
void AVLTree<T, comparator>::Remove(const int& position) {
    root = removeInternal(root, position);
}
// ----------------------------------------------------------------------------

int main() {
    AVLTree<int> avlTree;
    std::vector<int> result;

    int command;
    int value;
    int n = 0;
    int position;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> command >> value;
        switch (command) {
            case 1: {
                position = 0;
                avlTree.Add(value, position);
                result.push_back(position);
                break;
            }
            case 2: {
                position = value;
                avlTree.Remove(position);
                break;
            }
            default:
                break;
        }
    }

    for (int item : result) {
        std::cout << item << std::endl;
    }

    return 0;
}








