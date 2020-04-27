#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_SIZE = 8;
const float REHASH_RATIO = 0.75;

// берем взаимно простые числа - 127 и 97
class Hasher {
private:
    int prime;
public:
    Hasher(int prime = 127): prime(prime){};

    int operator()(const std::string& data, const int& size) {
        int hash = 0;
        for (char i : data) {
            hash = (hash * prime + i) % size;
        }
        return hash;
    }
};

// второе хэширование определяет шаг, если ячейка занята
class SecondHasher {
private:
    int prime;
public:
    SecondHasher(int prime = 97): prime(prime){};

    int operator()(const std::string& data, const int& size) {
        int hash = 0;
        for (int i = 0; i < data.size(); i++) {
            hash = (hash * prime + data[i]) % size;
        }
        return (2 * hash + 1) % size;
    }
};


template <typename T>
struct HashTableNode {
    HashTableNode(const T& key) : data(key), isDel(false) {}

    T data;
    bool isDel; //значение удалено? -> чтобы не потерять ноды с таким же хешем
};


template <typename T, typename Hasher, typename SecondHasher>
class HashTable {
public:
    HashTable() : size(0), table(DEFAULT_SIZE, nullptr) {};
    ~HashTable();

    bool Add(const T& key);
    bool Delete(const T& key);
    bool Has(const T& key);

    void Show();
private:
    void resizeTable();

    Hasher hasher;
    SecondHasher secondHasher;

    int size;
    std::vector<HashTableNode<T>*> table;
};

template <typename T, typename Hasher, typename SecondHasher>
HashTable<T, Hasher, SecondHasher>::~HashTable() {
    for (int i = 0; i < table.size(); i++) {
        HashTableNode<T> *node = table[i];
        delete node;
    }
}


template <typename T, typename Hasher, typename SecondHasher>
void HashTable<T, Hasher, SecondHasher>::resizeTable() {
    // сохраним уже вставленные значения
    auto temp = std::move(table);

    // расширим текущую таблицу в два раза
    table = std::move(std::vector<HashTableNode<T>*>(temp.size() * 2, nullptr));

    for (int i = 0; i < temp.size(); i++) {
        // добавим в новую таблицу старые действующие значения по новым хешам
        HashTableNode<T> *node = temp[i];

        if (node != nullptr && !node->isDel) {
            Add(node->data);
            // контролируем размер
            size--;
        }
    }
}


template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Add(const T &key) {
    // если пороговый коэф. заполненности таблицы достигнут, то перехешируем
    if (size >= table.size() * REHASH_RATIO) {
        resizeTable();
    }
    // если при вставке нашли такой же ключ, вернем false
    if (Has(key)) {
        return false;
    }

    int secondHash = secondHasher(key, table.size());
    int firstHash = hasher(key, table.size());
    int hash = firstHash;

    for (int i = 0; i < table.size() && table[hash] != nullptr; ++i) {

        if (table[hash]->isDel) {
            table[hash]->data = key;
            table[hash]->isDel = false;
            size++;
            return true;
        }

        hash = (firstHash + i * secondHash) % table.size();
    }

    // если не нашли значение, вместо которого вставлять, то с хэшом все нормально
    // иначе вставить на место удаленного
    table[hash] = new HashTableNode<T>(key);
    size++;
    return true;
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Delete(const T &key) {
    int secondHash = secondHasher(key, table.size());
    int firstHash = hasher(key, table.size());
    int hash = firstHash;

    for (int i = 0; i < table.size(); ++i) {
        if (table[hash] == nullptr) {
            return false;
        }

        if (table[hash]->data == key && !table[hash]->isDel) {
            table[hash]->isDel = true;
            size--;
            return true;
        }

        hash = (firstHash + i * secondHash) % table.size();
    }

    return false;
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Has(const T &key) {
    int secondHash = secondHasher(key, table.size());
    int firstHash = hasher(key, table.size());
    int hash = firstHash;

    int i = 0;

    while (table[hash] != nullptr && i < table.size()) {
        if (table[hash]->data == key && !table[hash]->isDel) {
            return true;
        }

        i++;
        hash = (firstHash + i * secondHash) % table.size();
    }

    return false;
}

template<typename T, typename Hasher, typename SecondHasher>
void HashTable<T, Hasher, SecondHasher>::Show() {
    for (int i = 0; i < table.size(); i++) {
        HashTableNode<T> *node = table[i];
        if (node != nullptr && !node->isDel) {
            std::cout << "[" << i << "] : " << node->data << std::endl;
        }
        if (node != nullptr && node->isDel) {
            std::cout << "[" << i << "] : " << node->data << " DEL" << std::endl;
        }
        if (node == nullptr) {
            std::cout << "[" << i << "] : " << "NULL" << std::endl;
        }
    }
}


int main() {
    HashTable<std::string, Hasher, SecondHasher> hTable;

    char command;
    std::string str;

    while (std::cin >> command >> str) {
        switch (command) {
            case '?': {
                std::cout << (hTable.Has(str) ? "OK" : "FAIL" ) << std::endl;
                hTable.Show();
                break;
            }
            case '+': {
                std::cout << (hTable.Add(str) ? "OK" : "FAIL" ) << std::endl;
                hTable.Show();
                break;
            }
            case '-': {
                std::cout << (hTable.Delete(str) ? "OK" : "FAIL" ) << std::endl;
                hTable.Show();
                break;
            }
            default:
                break;
        }
    }

    return 0;
}

