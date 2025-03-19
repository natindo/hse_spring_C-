#include "graph.h"
#include <vector>
#include <queue>

std::vector<int> bfs(const std::vector<std::vector<int>>& adjMatrix, int start) {
    const int n = static_cast<int>(adjMatrix.size());
    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        const int u = q.front();
        q.pop();
        for (int v = 0; v < n; ++v) {
            if (adjMatrix[u][v] == 1 && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}
