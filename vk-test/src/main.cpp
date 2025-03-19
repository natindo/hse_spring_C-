#include <iostream>
#include "graph_io.h"
#include "graph.h"



int main() {
    const std::string filename = "graph.txt";
    GraphData graphData = readGraphFromFile(filename);

    for (std::vector<int> distances = bfs(graphData.adjMatrix, graphData.start);
        const int d : distances) {
        std::cout << d << std::endl;
    }

    return 0;
}
