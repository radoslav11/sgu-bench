#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

/**
 * Problem: 226. Colored graph
 * Task: Find the shortest path from vertex 1 to N in a directed graph with edge colors {1, 2, 3}.
 * Constraint: No two successive edges can have the same color.
 * 
 * Approach:
 * Use BFS to find the shortest path in an unweighted graph.
 * Since the validity of the next edge depends on the color of the current edge,
 * we expand the state space. A state is defined by (current_vertex, last_edge_color).
 * 
 * State space: (N vertices) * (4 possible last_edge_colors: 0 for start, 1, 2, 3).
 * Total states: N * 4 = 200 * 4 = 800.
 * Complexity: O(N * 4 + M * 4), where M is the number of edges.
 * Given N <= 200 and M <= N^2 = 40,000, the total complexity is approximately 160,000 operations.
 * This is well within the 0.25s time limit and 4096 KB memory limit.
 */

using namespace std;

// Edge structure to store destination vertex and its color.
struct Edge {
    int to;
    int color;
};

// State structure to store current vertex and the color of the edge used to reach it.
struct State {
    int u;
    int last_color;
};

// Global arrays to avoid stack overflow and fit within memory limits.
// dist[u][c] stores the shortest distance to reach vertex u using an edge of color c.
int dist[205][4];
vector<Edge> adj[205];

int main() {
    int N, M;
    // Reading the number of vertices N and the number of edges M.
    if (scanf("%d %d", &N, &M) != 2) return 0;

    // Building the adjacency list.
    for (int i = 0; i < M; ++i) {
        int u, v, c;
        if (scanf("%d %d %d", &u, &v, &c) == 3) {
            adj[u].push_back({v, c});
        }
    }

    // Initializing the distance array with -1 (meaning unvisited).
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j < 4; ++j) {
            dist[i][j] = -1;
        }
    }

    // BFS initialization.
    // State (1, 0) represents starting at vertex 1 with no incoming edge (color 0).
    queue<State> q;
    dist[1][0] = 0;
    q.push({1, 0});

    while (!q.empty()) {
        State curr = q.front();
        q.pop();

        // Iterate through all edges outgoing from the current vertex.
        for (const auto& edge : adj[curr.u]) {
            // Ensure the next edge's color is different from the last edge's color.
            if (edge.color != curr.last_color) {
                // If the state (neighbor vertex, color of current edge) hasn't been visited yet.
                if (dist[edge.to][edge.color] == -1) {
                    dist[edge.to][edge.color] = dist[curr.u][curr.last_color] + 1;
                    q.push({edge.to, edge.color});
                }
            }
        }
    }

    // Find the minimum distance to reach the N-th vertex across all possible last-edge colors.
    int min_dist = -1;
    for (int c = 1; c <= 3; ++c) {
        if (dist[N][c] != -1) {
            if (min_dist == -1 || dist[N][c] < min_dist) {
                min_dist = dist[N][c];
            }
        }
    }

    // Output the result: shortest path length or -1 if unreachable.
    printf("%d\n", min_dist);

    return 0;
}

