#include "graph_io.h"
#include <fstream>
#include <stdexcept>

GraphData readGraphFromFile(const std::string &filename) {
    GraphData graphData;
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    int m;
    fin >> graphData.n >> m;
    if (graphData.n <= 0 || m < 0) {
        throw std::runtime_error("Некорректные данные о количестве вершин или ребер.");
    }

    graphData.adjMatrix.assign(graphData.n, std::vector<int>(graphData.n, 0));

    for (int i = 0; i < m; ++i) {
        int u, v;
        fin >> u >> v;
        if (u < 0 || u >= graphData.n || v < 0 || v >= graphData.n) {
            throw std::runtime_error("Некорректный индекс вершины");
        }
        graphData.adjMatrix[u][v] = 1;
        graphData.adjMatrix[v][u] = 1;
    }

    fin >> graphData.start;
    if (graphData.start < 0 || graphData.start >= graphData.n) {
        throw std::runtime_error("Некорректный номер стартовой вершины");
    }

    return graphData;
}
