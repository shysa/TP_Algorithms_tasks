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

template <typename T, typename Hasher, typename SecondHasher>
class HashTable {
public:
    HashTable(int initial_size = DEFAULT_SIZE) : size(0), table(initial_size, "NULL") {};
    ~HashTable() = default;

    bool Add(const T& key);
    bool Delete(const T& key);
    bool Has(const T& key);

    void Show();
private:
    void resizeTable();

    Hasher hasher;
    SecondHasher secondHasher;
    int size;
    std::vector<T> table;
};


template <typename T, typename Hasher, typename SecondHasher>
void HashTable<T, Hasher, SecondHasher>::resizeTable() {
    std::vector<std::string> newTable(table.size() * 2, "NULL");

    for (int i = 0; i < table.size(); i++) {
        // для каждой ноды старой таблицы вычисляем хэши в новой таблице
        if (table[i] != "NULL" && table[i] != "DEL") {
            int newHash = hasher(table[i]) % newTable.size();
            int newSecondHash = secondHasher(table[i]) % newTable.size();

            // если встречаем коллизию в новой таблице по новому хэшу, то опять-таки вставляем с шагом
            int j = 0;
            while ( newTable[j] != "NULL" && j < newTable.size() ) {
                newHash = (newHash + newSecondHash) % newTable.size();
                j++;
            }

            newTable[newHash] = table[i];
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

    int i = 0;
    // первое удаленное значение
    int hashDeleted = -1;

    while ( table[hash] != "NULL" && i < table.size() ) {
        // если при вставке нашли такой же ключ, вернем false
        if (table[hash] == key) {
            return false;
        }
        // запоминаем место удаленного значения
        if (table[hash] == "DEL" && hashDeleted == -1) {
            hashDeleted = hash;
        }

        hash = (hash + secondHash) % table.size();
        i++;
    }

    // если не нашли значение, вместо которого вставлять, то с хэшом все нормально
    // иначе вставить на место удаленного
    if (hashDeleted == -1) {
        table[hash] = key;
    } else {
        table[hashDeleted] = key;
    }
    size++;
    return true;
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Delete(const T &key) {
    int hash = hasher(key) % table.size();
    int secondHash = secondHasher(key) % table.size();

    int i = 0;

    while (table[hash] != "NULL" && i < table.size()) {
        if (table[hash] == key) {
            break;
        }
        // иначе проходим по другим возможным хэшам
        hash = (hash + secondHash) % table.size();
        i++;
    }

    // если значения нет и не было - выходим
    if (table[hash] == "NULL") {
        return false;
    }

    size--;
    table[hash] = "DEL";

    return true;
}

template <typename T, typename Hasher, typename SecondHasher>
bool HashTable<T, Hasher, SecondHasher>::Has(const T &key) {
    int hash = hasher(key) % table.size();
    int secondHash = secondHasher(key) % table.size();

    int i = 0;

    while ( table[hash] != "NULL" && i < table.size() ) {
        if (table[hash] == key) {
            return true;
        }
        hash = (hash + secondHash) % table.size();
        i++;
    }
    return false;
}

template<typename T, typename Hasher, typename SecondHasher>
void HashTable<T, Hasher, SecondHasher>::Show() {
    for (int i = 0; i < table.size(); i++) {
        std::cout << "[" << i << "] : " << table[i] << std::endl;
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

