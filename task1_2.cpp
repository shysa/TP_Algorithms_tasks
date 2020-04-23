#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_SIZE = 8;
const int REHASH_RATIO = 3;

// берем взаимно простые числа - 127 и 97
// 127 для того чтобы не было неожиданных неприятностей, если размер кратен 2
class Hasher {
private:
    int prime;
public:
    Hasher(int prime = 127): prime(prime){};

    int operator()(const std::string& data) {
        int hash = 0;
        for (int i = 0; i < data.size(); i++) {
            hash = hash * prime + data[i];
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

    int operator()(const std::string& data) {
        int hash = 0;
        for (int i = 0; i < data.size(); i++) {
            hash = hash * prime + data[i];
        }
        return hash;
    }
};


template <typename T>
struct HashTableNode {
    HashTableNode() : isDel(false) {};
    HashTableNode(const T& key, HashTableNode<T> *next) : data(key), isDel(false) {}

    T data;
    bool isDel; //значение удалено? -> чтобы не потерять ноды с таким же хешем
};

template <typename T, typename Hasher, typename SecondHasher>
class HashTable {
public:
    HashTable(int initial_size = DEFAULT_SIZE) : size(0), table(initial_size, nullptr) {};
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
    std::vector<HashTableNode<T>*> newTable(table.size() * 2, nullptr);

    for (int i = 0; i < table.size(); i++) {
        HashTableNode<T> *node = table[i];

        // для каждой ноды старой таблицы вычисляем хэши в новой таблице
        if (node != nullptr && !node->isDel) {
            int newHash = hasher(node->data) % newTable.size();
            int newSecondHash = secondHasher(node->data) % newTable.size();

            HashTableNode<T> *newNode = newTable[newHash];

            // если встречаем коллизию в новой таблице по новому хэшу, то опять-таки вставляем с шагом
            int j = 0;
            while ( newNode != nullptr && j < newTable.size() ) {
                newHash = (newHash + newSecondHash) % newTable.size();
                newNode = newTable[newHash];
                j++;
            }

            newTable[newHash] = node;
        }
    }
    table = std::move(newTable);
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Add(const T &key) {
    // если пороговый коэф. заполненности таблицы достигнут, то перехешируем
    if (size > table.size() * REHASH_RATIO) {
        resizeTable();
    }
    int hash = hasher(key) % table.size();
    int secondHash = secondHasher(key) % table.size();

    HashTableNode<T> *node = table[hash];
    int i = 0;
    // первое удаленное значение
    int hashDeleted = -1;

    while ( node != nullptr && i < table.size() ) {
        // если при вставке нашли такой же ключ, вернем false
        if (node->data == key && !node->isDel) {
            return false;
        }
        // запоминаем место удаленного значения
        if (node->isDel && hashDeleted == -1) {
            hashDeleted = hash;
        }

        hash = (hash + secondHash) % table.size();
        node = table[hash];
        i++;
    }

    // если не нашли значение, вместо которого вставлять, то с хэшом все нормально
    // иначе вставить на место удаленного
    if (hashDeleted == -1) {
        table[hash] = new HashTableNode<T>(key, table[hash]);
    } else {
        node = table[hashDeleted];
        node->isDel = false;
        node->data = key;
    }
    size++;
    return true;
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Delete(const T &key) {
    int hash = hasher(key) % table.size();
    int secondHash = secondHasher(key) % table.size();

    HashTableNode<T> *node = table[hash];
    int i = 0;

    while (node != nullptr && i < table.size()) {
        // если узел по полученному хэшу не удален и нужный, выходим
        if ( node->data == key && !node->isDel ) {
            break;
        }
        // иначе проходим по другим возможным хэшам
        hash = (hash + secondHash) % table.size();
        node = table[hash];
        i++;
    }

    // если значения нет и не было - выходим
    if (node == nullptr) {
        return false;
    }

    size--;
    node->isDel = true;

    return true;
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Has(const T &key) {
    int hash = hasher(key) % table.size();
    int secondHash = secondHasher(key) % table.size();

    HashTableNode<T> *node = table[hash];
    int i = 0;

    while ( node != nullptr && i < table.size() ) {
        if (node->data == key && !node->isDel) {
            return true;
        }
        hash = (hash + secondHash) % table.size();
        node = table[hash];
        i++;
    }
    return false;
}

template<typename T, typename Hasher, typename SecondHasher>
void HashTable<T, Hasher, SecondHasher>::Show() {
    HashTableNode<T> *node;
    for (int i = 0; i < table.size(); i++) {
        node = table[i];
        if (node != nullptr && !node->isDel) {
            std::cout << "[" << i << "] : " << node->data << std::endl;
        }
    }
}


int main() {
    HashTable<std::string, Hasher, SecondHasher> hTable;

    char command;
    std::string str;
    bool end = false;

    while (!end) {
        std::cin >> command >> str;
        switch (command) {
            case '?': {
                std::cout << (hTable.Has(str) ? "OK" : "FAIL" ) << std::endl;
                break;
            }
            case '+': {
                std::cout << (hTable.Add(str) ? "OK" : "FAIL" ) << std::endl;
                break;
            }
            case '-': {
                std::cout << (hTable.Delete(str) ? "OK" : "FAIL" ) << std::endl;
                break;
            }
            case 'e' : {
                end = true;
                break;
            }
            default:
                break;
        }
    }

    hTable.Show();

    return 0;
}

