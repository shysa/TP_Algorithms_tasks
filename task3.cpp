#include <iostream>
#include <vector>
#include <cassert>
#include <set>

// ------------------------------- LIST --------------------------------------------
class ListGraph {
private:
    struct vertexData {
        // текущий кратчайший путь
        // для всех +inf, кроме 0й вершины
        size_t currentMinPath;

        // в начале каждой вершине ставим отсутствие родительской вершины = -1
        int parent;

        vertexData() : currentMinPath(-1), parent(-1) {}
    };

    // смежная вершина + вес ребра
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
public:
    ListGraph(int size): adjacencyLists(size) {};

    ~ListGraph() = default;

    void AddEdge(int from, int to, int weight) {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        assert(weight >= 0);

        adjacencyLists[from].push_back(std::make_pair(weight, to));
        adjacencyLists[to].push_back(std::make_pair(weight, from));
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    // -------------------------------- TASK ----------------------------------------
    int findMinPath(int from, int to) {
        std::vector<vertexData> states(adjacencyLists.size());

        // pair(d[i], i)
        std::set<std::pair<int, int>> queue;

        queue.insert({0, from});
        states[from].currentMinPath = 0;
        states[from].parent = from;

        while (!queue.empty()) {
            std::pair<int, int> current = *queue.begin();
            queue.erase(queue.begin());

            // pair(weight, i)
            std::vector<std::pair<int, int>> next = GetNextVertices(current.second);
            for (const auto& vertex : next) {
                if (states[current.second].currentMinPath + vertex.first < states[vertex.second].currentMinPath) {
                    // релаксируем ребро в set
                    queue.erase({states[vertex.second].currentMinPath, vertex.first});

                    queue.insert(vertex);
                    states[vertex.second].currentMinPath = states[current.second].currentMinPath + vertex.first;
                    states[vertex.second].parent = current.first;

                }
            }
        }

        return states[to].currentMinPath;
    }
};


int main() {
    int N = 0;      // количество городов
    int M = 0;      // количество дорог
    int from;
    int to;
    int weight;

    std::cin >> N >> M;
    ListGraph listGraph(N);
    for (int i = 0; i < M; i++) {
        std::cin >> from >> to >> weight;
        listGraph.AddEdge(from, to, weight);
    }

    std::cin >> from >> to;
    std::cout << listGraph.findMinPath(from, to);

    return 0;
}