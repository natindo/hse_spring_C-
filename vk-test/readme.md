# GraphShortestPath

This project implements a C++ program that calculates the shortest path distances from a given starting vertex in an undirected graph using the Breadth-First Search (BFS) algorithm. The graph is represented as an adjacency matrix, and graph data is read from a file.

## Features

- **Graph Input:** Reads graph data from a file (`graph.txt`) with a fixed format.
- **BFS Algorithm:** Computes shortest paths using the BFS algorithm.
- **Adjacency Matrix:** Graph is stored as a full \( n \times n \) matrix.
- **Exception Handling:** Uses exceptions to handle invalid input data.
- **Unit Testing:** Includes unit tests using GoogleTest.
- **CMake Build:** Uses CMake for project configuration and building.
- **CI/CD Ready:** Configured for CI/CD (e.g., GitHub Actions).

## Input File Format

The input file (default: `graph.txt`) must have the following format:

```
<number of vertices>
<number of edges>
<edge 1: vertex1 vertex2>
...
<edge m: vertex1 vertex2>
<start vertex>
```

## Building the Project

Ensure you have CMake (version ≥ 3.20) and a C++ compiler with C++20 support.

### Using the Shell Script

Make the script executable and run it:

```bash
chmod +x run.sh
./run.sh
```

This script will create a `build` directory, run CMake, build the project, and execute the main binary.

## Running Tests

After building, run the tests using CTest:

```bash
cd build
ctest --verbose
```

Or run the test binary directly:

```bash
./runTests
```
