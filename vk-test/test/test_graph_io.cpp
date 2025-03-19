#include "gtest/gtest.h"
#include "graph_io.h"
#include <fstream>
#include <cstdio>

TEST(GraphIOTest, ValidGraphFile) {
    std::string filename = "temp_graph.txt";
    std::ofstream fout(filename);
    fout << "4\n" << "3\n"
         << "0 1\n"
         << "1 2\n"
         << "2 3\n"
         << "0\n";
    fout.close();

    GraphData data = readGraphFromFile(filename);
    EXPECT_EQ(data.n, 4);
    EXPECT_EQ(data.start, 0);

    std::vector<std::vector<int>> expected = {
        {0, 1, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {0, 0, 1, 0}
    };
    EXPECT_EQ(data.adjMatrix, expected);

    std::remove(filename.c_str());
}

TEST(GraphIOTest, FileNotFound) {
    EXPECT_THROW({
        readGraphFromFile("nonexistent_file.txt");
    }, std::runtime_error);
}

TEST(GraphIOTest, InvalidStartVertex) {
    std::string filename = "temp_invalid.txt";
    std::ofstream fout(filename);
    fout << "3\n" << "1\n"
         << "0 1\n"
         << "5\n";
    fout.close();

    EXPECT_THROW({
        readGraphFromFile(filename);
    }, std::runtime_error);

    std::remove(filename.c_str());
}
