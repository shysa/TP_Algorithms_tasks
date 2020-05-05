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

        node->nodes++;

        if (cmp(data, node->data)) {
            // итоговая позиция точно не меньше чем nodes(right) + 1
            position += getNodes(node->right) + 1;
            node->left = addInternal(node->left, data, position);
        } else {
            node->right = addInternal(node->right, data, position);
        }

        return doBalance(node);
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

    Node* removeInternal(Node *node, const int& position) {
        if (!node) {
            return nullptr;
        }

        if (position >= node->nodes) {
            return node;
        }

        int current = 0;
        std::stack<Node *> nodes;

        while (true) {
            int nodes_right = getNodes(node->right);

            if (position - current > nodes_right) {
                nodes.push(node);
                node = node->left;
                current += nodes_right + 1;
            }
            else if (position - current < nodes_right) {
                if (node->right) {
                    nodes.push(node);
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

                // если у удаляемого элемента не было правого и левого потомков,
                // то просто поправим предыдущий узел
                if (!right) {
                    if (!left) {
                        // берем предыдущий пройденный узел
                        if (!nodes.empty()) {
                            node = nodes.top();
                            nodes.pop();

                            // чистим связи на удаленный узел
                            if (node->data > key) {
                                node->left = nullptr;
                            } else {
                                node->right = nullptr;
                            }
                            // и корректируем показатель nodes
                            node->nodes--;
                        } else {
                            return nullptr;
                        }
                    } else {
                        // если был левый потомок, встаем на него
                        node = left;
                    }
                } else {
                    // если правый потомок существует, то заменяем на минимальный из правого поддерева
                    Node *min = findMin(right);
                    min->right = removeMin(right);
                    min->left = left;

                    fixNodes(min);
                    return doBalance(min);
                }
                break;
            }
        }

        // исправляем nodes у всех пройденных узлов
        while (!nodes.empty()) {
            Node *tmp = nodes.top();
            node->nodes--;

            if (tmp->data > node->data) {
                tmp->left = node;
            } else {
                tmp->right = node;
            }

            node = doBalance(node);
            nodes.pop();
        }

        return node;

        /*

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
        return doBalance(node);*/
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
    void Remove(const int& position);
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








