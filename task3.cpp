#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}
    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

// ------------------------------- LIST --------------------------------------------
class ListGraph: public IGraph {
private:
    struct vertexData {
        // текущее кратчайшее расстояние до вершины
        // для всех +inf, кроме 0й вершины
        size_t currentMinPath;

        // в начале каждой вершине ставим отсутствие родительской вершины = -1
        int parent;

        // подсчет количества кратчайших путей
        // для всех 0, кроме 0й вершины
        int paths;

        vertexData() : currentMinPath(-1), parent(-1), paths(0) {}
    };

    std::vector<std::vector<int>> adjacencyLists;
public:
    ListGraph(int size): adjacencyLists(size) {};
    ListGraph(const IGraph& graph): adjacencyLists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override  {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());

        adjacencyLists[from].push_back(to);
        adjacencyLists[to].push_back(from);
    }

    int VerticesCount() const override {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++) {
            for (int to : adjacencyLists[from]) {
                if (to == vertex) {
                    prevVertices.push_back(from);
                }
            }
        }

        return prevVertices;
    }

    // -------------------------------- TASK ----------------------------------------
    int coutMinPaths(int from, int to) {
        std::vector<vertexData> states(adjacencyLists.size());
        std::queue<int> queue;

        queue.push(from);
        states[from].currentMinPath = 0;
        states[from].paths = 1;

        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();

            std::vector<int> next = GetNextVertices(current);
            for (int vertex : next) {
                // из текущей вершины можем попасть в следующую дешевле, чем по уже имеющемуся пути?
                if (states[current].currentMinPath + 1 < states[vertex].currentMinPath) {
                    queue.push(vertex);
                    states[vertex].currentMinPath = states[current].currentMinPath + 1;
                    states[vertex].parent = current;
                    states[vertex].paths = states[current].paths;
                } else if (states[current].currentMinPath + 1 == states[vertex].currentMinPath) {
                    states[vertex].paths += states[current].paths;
                }
            }
        }

        return states[to].paths;
    }
};


int main() {
    int v = 0;      // Кол-во вершин
    int e = 0;      // Кол-во ребер
    int from;
    int to;

    std::cin >> v >> e;
    ListGraph listGraph(v);
    for (int i = 0; i < e; i++) {
        std::cin >> from >> to;
        listGraph.AddEdge(from, to);
    }

    std::cin >> from >> to;
    std::cout << listGraph.coutMinPaths(from, to);

    return 0;
}