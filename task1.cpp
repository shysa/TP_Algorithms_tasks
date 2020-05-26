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
};
// ------------------------------- MATRIX --------------------------------------------
class MatrixGraph: public IGraph {
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
public:
    MatrixGraph(int size): adjacencyMatrix(size, std::vector<bool>(size, false)) {};
    MatrixGraph(const IGraph& graph): adjacencyMatrix(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false)) {
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
        std::vector<int> nextVertices;
        for (int i = 0; i < adjacencyMatrix.size(); i++) {
            if (adjacencyMatrix[vertex][i]) {
                nextVertices.push_back(i);
            }
        }

        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        std::vector<int> prevVertices;

        for (int i = 0; i < adjacencyMatrix.size(); i++) {
            if (adjacencyMatrix[i][vertex]) {
                prevVertices.push_back(i);
            }
        }

        return prevVertices;
    }
};
// ------------------------------- SET --------------------------------------------
class SetGraph: public IGraph {
private:
    std::vector<std::unordered_set<int>> adjacencySet;
public:
    SetGraph(int size): adjacencySet(size, std::unordered_set<int>(size)) {};
    SetGraph(const IGraph& graph): adjacencySet(graph.VerticesCount(), std::unordered_set<int>(graph.VerticesCount())) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            std::vector<int> vertices = graph.GetNextVertices(i);
            for (auto next : vertices) {
                AddEdge(i, next);
            }
        }
    }
    ~SetGraph() override = default;

    void AddEdge(int from, int to) override  {
        assert(0 <= from && from < adjacencySet.size());
        assert(0 <= to && to < adjacencySet.size());

        adjacencySet[from].insert(to);
        adjacencySet[to].insert(from);
    }

    int VerticesCount() const override {
        return (int)adjacencySet.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencySet.size());

        std::vector<int> nextVertices;
        for (int next : adjacencySet[vertex]) {
            nextVertices.push_back(next);
        }

        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencySet.size());

        std::vector<int> prevVertices;
        for (int i = 0; i < adjacencySet.size(); i++) {
            if (adjacencySet[i].count(vertex) > 0) {
                prevVertices.push_back(i);
                break;
            }
        }

        return prevVertices;
    }
};
// ------------------------------- ARC --------------------------------------------
class ArcGraph: public IGraph {
private:
    std::vector<std::pair<int, int>> adjacencyPairs;
public:
    ArcGraph(int size): adjacencyPairs(size) {};
    ArcGraph(const IGraph& graph): adjacencyPairs(0) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            std::vector<int> vertices = graph.GetNextVertices(i);
            for (auto next : vertices) {
                AddEdge(i, next);
            }
        }
    }
    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override  {
        // если пара уже существует, не добавляем
        for (auto pair : adjacencyPairs) {
            if (((pair.first == from) && (pair.second == to)) || ((pair.second == from) && (pair.first == to))) {
                return;
            }
        }

        adjacencyPairs.emplace_back(from, to);
    }

    int VerticesCount() const override {
        return (int)adjacencyPairs.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyPairs.size());

        std::vector<int> nextVertices;
        for (auto pair : adjacencyPairs) {
            if (pair.first == vertex) {
                nextVertices.push_back(pair.second);
            }
        }

        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyPairs.size());

        std::vector<int> prevVertices;
        for (auto pair : adjacencyPairs) {
            if (pair.second == vertex) {
                prevVertices.push_back(pair.first);
            }
        }

        return prevVertices;
    }
};
// --------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& os, IGraph &graph) {
    for (int i = 0; i < 5; i++) {
        os << "[" << i << "]: ";
        for (int vertex : graph.GetNextVertices(i)) {
            os << vertex << " ";
        }
        os << std::endl;
    }
    return os;
}
// --------------------------------------------------------------------------------

//  GRAPH:
//  0 --- 1
//  |    / \
//  4 - 3 - 2

int main() {
    ListGraph listGraph(5);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 4);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(2, 3);
    listGraph.AddEdge(3, 4);
    std::cout << "list:\n" << listGraph << std::endl;

    MatrixGraph matrixGraph(listGraph);
    std::cout << "matrix:\n" << listGraph << std::endl;

    SetGraph setGraph(matrixGraph);
    std::cout << "set:\n" << listGraph << std::endl;

    ArcGraph arcGraph(setGraph);
    std::cout << "arc:\n" << listGraph << std::endl;

    std::cout << "BFS for graphs:" << std::endl;
    std::cout << "[list] ";
    mainBFS(listGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "[matrix] ";
    mainBFS(matrixGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "[set] ";
    mainBFS(setGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "[arc] ";
    mainBFS(arcGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    /*
    mainBFS(listGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(listGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex : topologicalSort(listGraph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    */

    return 0;
}
