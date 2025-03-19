#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct GraphData {
    int n;
    std::vector<std::vector<int>> adjMatrix;
    int start;

    GraphData() : n(0), start(-1) {}

    GraphData(const GraphData& other) = default;

    GraphData(GraphData&& other) noexcept
        : n(other.n), adjMatrix(std::move(other.adjMatrix)), start(other.start) {
        std::cout << "asd" << std::endl;
        other.n = 0;
        other.start = -1;
    }

    GraphData& operator=(const GraphData& other) {
        if (this != &other) {
            n = other.n;
            adjMatrix = other.adjMatrix;
            start = other.start;
        }
        return *this;
    }

    GraphData& operator=(GraphData&& other) noexcept {
        if (this != &other) {
            n = other.n;
            adjMatrix = std::move(other.adjMatrix);
            start = other.start;
            other.n = 0;
            other.start = -1;
        }
        return *this;
    }

    ~GraphData() = default;
};

GraphData readGraphFromFile(const std::string &filename);

#endif // GRAPH_IO_H
