#include <iostream>
#include <vector>
#include <stack>
#include <map>

class DSU {
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;

    friend std::ostream& operator<< (std::ostream& os, DSU &dsu);
public:
    DSU(size_t size): rank(size, 1), parent(size) {
        for (auto i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    // возвращаем элемент и заодно оптимизация
    size_t findSet(size_t u) {
        std::stack<size_t> stack;
        size_t current = u;

        stack.push(current);
        while (current != parent[current]) {
            stack.push(parent[current]);
            current = parent[current];
        }
        // после выхода из цикла current указывает на root
        size_t root = current;
        while (!stack.empty()) {
            // каждую пройденную вершину переподцепляем к представительскому элементу, высота дерева <= 2
            parent[stack.top()] = root;
            stack.pop();
        }

        return root;
    }

    // объединение множеств
    void unionSet(size_t u, size_t v) {
        // N = представительскому элементу множества, где лежит N
        u = findSet(u);
        v = findSet(v);

        if (u != v) {
            // u гарантированно больший чем v
            if (rank[u] < rank[v]) {
                std::swap(u, v);
            }
            parent[v] = u;
            rank[u] += rank[v];
        }
    }
};

std::ostream& operator<< (std::ostream& os, DSU &dsu) {
    std::map<size_t, std::vector<size_t>> sets;

    for (int i = 0; i < dsu.parent.size(); i++) {
        sets[dsu.findSet(i)].push_back(i);
    }

    for (auto &kv : sets) {
        os << kv.first << " [rank = " << dsu.rank[kv.first] << " ]: ";
        for (auto j = 0; j = kv.second.size(); j++) {
            os << kv.second[j];
            if (j != kv.second.size() - 1) {
                os << ", ";
            }
        }
        os << std::endl;
    }
    return os;
}

int main() {
    DSU dsu(10);
    std::cout << dsu << std::endl;
    std::cout << std::endl;

    dsu.unionSet(4, 9);
    std::cout << dsu << std::endl;

    /*
     * pseudocode:
     * Kruskal(graph) {
     * result = {}
     * foreach edge in Edges {
     *      makeset(edge)
     * }
     * Edges = sort(Edges) <- реба в порядке возрастания веса
     * foreach (u, v) in Edges {
     *     if findset(u) != findset(v) { result.add(u,v); unionset(u,v) }
     * }
     *
     * return result <- содержит все ребра
     */

    return 0;
}