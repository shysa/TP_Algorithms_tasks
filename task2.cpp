#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <functional>
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
        int depth;
        int paths;
        vertexData() : depth(0), paths(0) {}
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
        // подсчет длины кратчайшего пути и количества min путей
        std::vector<vertexData> states(adjacencyLists.size());
        // очередь обхода вершин
        std::queue<int> queue;

        queue.push(from);
        states[from].paths = 1;

        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();

            // просматриваем все следующие вершины
            std::vector<int> vertices = GetNextVertices(current);
            for (int vertex : vertices) {
                if (states[vertex].paths == 0) {
                    queue.push(vertex);
                    states[vertex].paths = states[current].paths;
                    states[vertex].depth = states[current].depth + 1;
                } else if (states[vertex].depth == states[current].depth + 1) {
                    states[vertex].paths += states[current].paths;
                }
            }
        }

        return states[to].paths;
    }
};

int main() {
    ListGraph listGraph(5);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 4);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(2, 3);
    listGraph.AddEdge(3, 4);

    return 0;
}