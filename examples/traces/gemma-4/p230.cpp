#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * There are N types of coins with values 1, 2, ..., N.
 * The weight of an i-burle coin is i grams.
 * Each box contains one of the N coins (one of each type).
 * There are M weighings, each stating that the coin in box P is lighter than the coin in box Q.
 * This implies the weight (and value) of the coin in box P is less than the coin in box Q.
 *
 * Let c_i be the value of the coin in the i-th box.
 * The constraint "box P is lighter than box Q" translates to c_P < c_Q.
 * This is a dependency problem that can be modeled as a directed graph where an edge 
 * from P to Q means c_P must be smaller than c_Q.
 * We need to find a permutation c_1, c_2, ..., c_N of the values 1, 2, ..., N
 * that satisfies these constraints.
 *
 * This is equivalent to finding a topological sort of the dependency graph.
 * If the graph contains a cycle, it's impossible to satisfy the constraints, 
 * and we should output "No solution".
 * Otherwise, we can perform a topological sort and assign the values 1, 2, ..., N 
 * in the order that boxes appear in the topological sorting.
 *
 * Complexity:
 * N <= 100, M <= 10000.
 * Topological sorting (Kahn's algorithm) takes O(N + M) time.
 * This is efficient enough for the 0.25s time limit.
 */

int main() {
    // Faster I/O for competitive programming.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    // Read the number of coin types N and number of weighings M.
    if (!(cin >> N >> M)) return 0;

    // Using an adjacency list to represent the dependency graph.
    // adj[p] stores the boxes that must contain a heavier coin than box p.
    vector<vector<int>> adj(N + 1);
    // in_degree[q] stores how many boxes are lighter than box q.
    vector<int> in_degree(N + 1, 0);

    for (int i = 0; i < M; ++i) {
        int p, q;
        if (cin >> p >> q) {
            adj[p].push_back(q);
            in_degree[q]++;
        }
    }

    // Kahn's algorithm for topological sorting.
    queue<int> q;
    // Start with boxes that have no dependencies (nothing is lighter than them).
    for (int i = 1; i <= N; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    vector<int> topo_order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);

        for (int v : adj[u]) {
            in_degree[v]--;
            // If a box has no more lighter dependency constraints, add it to the queue.
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    // If topo_order does not contain all N boxes, a cycle exists in the graph.
    if ((int)topo_order.size() < N) {
        cout << "No solution" << endl;
    } else {
        // coins[i] stores the value of the coin assigned to box i.
        vector<int> coins(N + 1);
        // Assign values 1, 2, ..., N to boxes according to the topological order.
        for (int i = 0; i < N; ++i) {
            // topo_order[i] is the i-th box in the sorted order (smallest to largest).
            // We assign it the value (i + 1).
            coins[topo_order[i]] = i + 1;
        }
        // Output the coin values in the order of the boxes (1 to N).
        for (int i = 1; i <= N; ++i) {
            cout << coins[i] << (i == N ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

