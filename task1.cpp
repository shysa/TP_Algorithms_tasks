#include <iostream>
#include <vector>
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

//  GRAPH:
//  0 --- 1
//  |    / \
//  4 - 3 - 2

// ------------------------------- LIST --------------------------------------------
// (0) -> 1 4
// (1) -> 0 2 3 4
// (2) -> 1 3
// (3) -> 1 2 4
// (4) -> 0 1 3
//
class ListGraph: public IGraph {
private:
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
};

// ------------------------------- MATRIX --------------------------------------------
//    0 1 2 3 4
//  ------------
// 0| 0 1 0 0 1
// 1| 1 0 1 1 0
// 2| 0 1 0 1 0
// 3| 0 1 1 0 1
// 4| 1 0 0 1 0
//
class MatrixGraph: public IGraph {
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
public:
    MatrixGraph(int size): adjacencyMatrix(size, std::vector<bool>(size, false)) {};
    MatrixGraph(const IGraph& graph): adjacencyMatrix(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            std::vector<int> vertices = graph.GetNextVertices(i);
            for (auto next : vertices) {
                AddEdge(i, next);
            }
        }
    }
    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override  {
        assert(0 <= from && from < adjacencyMatrix.size());
        assert(0 <= to && to < adjacencyMatrix.size());

        // симметричность матрицы?
        adjacencyMatrix[from][to] = true;
        adjacencyMatrix[to][from] = true;
    }

    int VerticesCount() const override {
        return (int)adjacencyMatrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        return adjacencyLists[vertex];
    }

    /*
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
    }*/
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func) {
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int currentVertex = qu.front();
        qu.pop();
        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, std::function<void(int)> func) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func) {
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void mainDFS(const IGraph &graph, std::function<void(int)> func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) DFS(graph, i, visited, func);
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted) {
    visited[vertex] = true;
    for (int nextVertex: graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) topologicalSortInternal(graph, nextVertex, visited, sorted);
    }
    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph) {
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) topologicalSortInternal(graph, i, visited, sorted);
    }
    return sorted;
}

int main(int argc, const char * argv[]) {
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    mainBFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

   /* for (int vertex : topologicalSort(graph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;*/

    /* ArcGraph arcGraph(graph);
     * SetGraph setGraph(arcGraph);
     * MatrixGraph mGraph(setGraph);
     * mainBFS(graph, [](int vertex){ std::cout « vertex « " ";});
     * std::cout « std::endl;
     * mainBFS(mGraph, [](int vertex){ std::cout « vertex « " ";});
     * std::cout « std::endl; */

    return 0;
}
