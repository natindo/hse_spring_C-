#include "gtest/gtest.h"
#include "graph.h"
#include <fstream>
#include <cstdio>
#include <graph_io.h>

// Test of correct reading of graph from file
TEST(ReadGraphTest, ValidGraphFile) {
    std::string filename = "temp_graph.txt";
    std::ofstream fout(filename);
    fout << "5\n" << "4\n";
    fout << "0 1\n";
    fout << "0 4\n";
    fout << "1 2\n";
    fout << "1 3\n";
    fout << "4\n";
    fout.close();

    GraphData data = readGraphFromFile(filename);
    EXPECT_EQ(data.n, 5);
    EXPECT_EQ(data.start, 4);

    std::vector<std::vector<int>> expected = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0}
    };
    EXPECT_EQ(data.adjMatrix, expected);

    std::remove(filename.c_str());
}

// Test for the situation when the file does not exist
TEST(ReadGraphTest, FileNotFound) {
    std::string filename = "nonexistent_file.txt";
    EXPECT_THROW(readGraphFromFile(filename), std::runtime_error);
}

// Test for incorrect сount
TEST(ReadGraphTest, InvalidGraphCount) {
    std::string filename = "temp_invalid_count.txt";
    std::ofstream fout(filename);
    fout << "-1\n" << "2\n";
    fout.close();

    EXPECT_THROW(readGraphFromFile(filename), std::runtime_error);
}

// Test for incorrect edge index
TEST(ReadGraphTest, InvalidEdgeIndex) {
    std::string filename = "temp_invalid_edge.txt";
    std::ofstream fout(filename);
    fout << "3\n" << "1\n";
    fout << "0 3\n";
    fout << "0\n";
    fout.close();

    EXPECT_THROW(readGraphFromFile(filename), std::runtime_error);
}

// Test for bfs algorithm
TEST(BFSTest, SimpleGraph) {
    std::vector<std::vector<int>> matrix = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0}
    };
    int start = 4;
    std::vector<int> result = bfs(matrix, start);
    std::vector<int> expected = {1, 2, 3, 3, 0};
    EXPECT_EQ(result, expected);
}

// Test with a single vertex
TEST(BFSTest, SingleVertex) {
    std::vector<std::vector<int>> graph = { {0} };
    int start = 0;
    std::vector<int> result = bfs(graph, start);
    std::vector<int> expected = {0};
    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
