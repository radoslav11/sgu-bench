#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Domino
 * The problem asks to arrange a given set of N domino pieces (each with two numbers 0-6)
 * in a line such that adjacent sides have the same number.
 * This is equivalent to finding an Eulerian path in a multigraph where:
 * - Vertices are the numbers 0-6.
 * - Each piece is an edge connecting the two numbers on it.
 * - Rotating a piece is equivalent to traversing an edge in a different direction.
 * 
 * An Eulerian path exists if:
 * 1. All edges belong to a single connected component.
 * 2. The number of vertices with an odd degree is either 0 or 2.
 * 
 * N is up to 100, and vertices are 0-6, making this problem very efficient to solve
 * using Hierholzer's algorithm for Eulerian paths.
 */

// Structure representing an edge in our graph
struct Edge {
    int to;      // The other end of the edge
    int id;      // Original index of the piece
    int u, v;    // Original numbers on the piece
};

// Structure representing a step in the Eulerian path
struct PathStep {
    int id;      // Piece index
    int u, v;    // Actual direction in which the edge was traversed
    int orig_u, orig_v; // Original orientation of the piece
};

// Graph and path variables
vector<Edge> adj[7];
bool used_edge[105];
vector<PathStep> path;

/**
 * Hierholzer's algorithm to find an Eulerian path.
 * This recursive implementation finds the path and builds it in reverse.
 */
void findEuler(int u) {
    while (!adj[u].empty()) {
        Edge e = adj[u].back();
        adj[u].pop_back();
        if (used_edge[e.id]) continue;
        used_edge[e.id] = true;
        
        // Explore the next vertex
        findEuler(e.to);
        
        // Store the piece and the direction it was used in the path
        // The path is built in reverse during the recursion's unwind phase.
        path.push_back({e.id, u, e.to, e.u, e.v});
    }
}

int main() {
    // Speed up input and output
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<int> degree(7, 0);
    for (int i = 1; i <= N; ++i) {
        int u, v;
        if (!(cin >> u >> v)) break;
        adj[u].push_back({v, i, u, v});
        adj[v].push_back({u, i, u, v});
        degree[u]++;
        degree[v]++;
    }

    // Determine the number of odd-degree vertices
    int odd_count = 0;
    vector<int> odd_nodes;
    for (int i = 0; i < 7; ++i) {
        if (degree[i] % 2 != 0) {
            odd_count++;
            odd_nodes.push_back(i);
        }
    }

    // Euler path conditions: 0 odd-degree vertices (circuit) or 2 (path)
    if (odd_count != 0 && odd_count != 2) {
        cout << "No solution" << endl;
        return 0;
    }

    int start_node = -1;
    if (odd_count == 2) {
        // If there are 2 odd-degree vertices, the path must start at one of them.
        start_node = odd_nodes[0];
    } else {
        // If all vertices have even degree, pick any vertex with edges.
        for (int i = 0; i < 7; ++i) {
            if (degree[i] > 0) {
                start_node = i;
                break;
            }
        }
    }

    // Perform DFS to find the Eulerian path
    if (start_node != -1) {
        findEuler(start_node);
    }

    // A solution is valid if all N pieces are included in the path
    if (path.size() != (size_t)N) {
        cout << "No solution" << endl;
    } else {
        // The path was built in reverse, so reverse it back to get left-to-right order
        reverse(path.begin(), path.end());
        for (const auto& step : path) {
            // Check if the traversed direction matches the original orientation
            if (step.u == step.orig_u && step.v == step.orig_v) {
                cout << step.id << " +" << "\n";
            } else {
                // Note: if orig_u == orig_v, step.u == step.orig_u will be true, giving '+'
                cout << step.id << " -" << "\n";
            }
        }
    }

    return 0;
}

