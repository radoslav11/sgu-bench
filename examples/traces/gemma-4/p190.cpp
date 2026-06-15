#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Problem Summary:
 * Given an NxN chessboard with some squares removed, we need to determine if the 
 * remaining squares can be perfectly covered with 2x1 dominoes. 
 * If so, we output "Yes" followed by the coordinates of horizontal and 
 * vertical dominoes. Otherwise, we output "No".
 * 
 * Approach:
 * This is a perfect matching problem in a bipartite graph.
 * Each non-removed square is a node. An edge exists between two adjacent 
 * non-removed squares. The chessboard is naturally bipartite: color 
 * squares based on (x + y) % 2. Dominoes always cover one even and one odd square.
 * 
 * Constraints:
 * N <= 40, so N^2 <= 1600.
 * Time limit: 0.25 sec (very tight).
 * Memory limit: 4096 KB (quite small).
 */

int N, P;
bool isRemoved[41][41];
vector<int> adj[1601];
int matchR[1601];
int vis[1601];
int timer = 0;

// Convert (x, y) coordinates to a unique integer ID
inline int get_id(int x, int y) {
    return (x - 1) * N + (y - 1);
}

// Convert an integer ID back to (x, y) coordinates
inline void get_coords(int id, int &x, int &y) {
    x = id / N + 1;
    y = id % N + 1;
}

// Standard DFS-based augmenting path algorithm for bipartite matching
bool dfs(int u) {
    for (int v : adj[u]) {
        if (vis[v] != timer) {
            vis[v] = timer;
            if (matchR[v] == -1 || dfs(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

struct Domino {
    int x, y;
};

int main() {
    // Optimize I/O speed for tight time limit
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> P)) return 0;

    // Resetting the removed squares array
    memset(isRemoved, 0, sizeof(isRemoved));
    for (int i = 0; i < P; ++i) {
        int x, y;
        cin >> x >> y;
        if (x >= 1 && x <= N && y >= 1 && y <= N) {
            isRemoved[x][y] = true;
        }
    }

    int available_count = 0;
    vector<int> S1; // Set of nodes where (x + y) is even
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (!isRemoved[i][j]) {
                available_count++;
                if ((i + j) % 2 == 0) {
                    S1.push_back(get_id(i, j));
                }
            }
        }
    }

    // A perfect matching is only possible if there is an even number of squares
    if (available_count % 2 != 0) {
        cout << "No" << endl;
        return 0;
    }

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    // Building the adjacency list for the bipartite graph
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (!isRemoved[i][j] && (i + j) % 2 == 0) {
                int u = get_id(i, j);
                for (int k = 0; k < 4; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 1 && ni <= N && nj >= 1 && nj <= N && !isRemoved[ni][nj]) {
                        adj[u].push_back(get_id(ni, nj));
                    }
                }
            }
        }
    }

    // Matching algorithms
    memset(matchR, -1, sizeof(matchR));
    int matching_count = 0;
    vector<bool> isMatchedS1(1601, false);

    // Greedy matching step to speed up the subsequent DFS steps
    for (int u : S1) {
        for (int v : adj[u]) {
            if (matchR[v] == -1) {
                matchR[v] = u;
                isMatchedS1[u] = true;
                matching_count++;
                break;
            }
        }
    }

    // Finding augmenting paths for the unmatched nodes in S1
    for (int u : S1) {
        if (!isMatchedS1[u]) {
            timer++;
            if (dfs(u)) {
                matching_count++;
            }
        }
    }

    // If the matching covers all available squares, we have a perfect matching
    if (matching_count * 2 != available_count) {
        cout << "No" << endl;
    } else {
        cout << "Yes" << endl;
        vector<Domino> horiz, vert;
        for (int v = 0; v < N * N; ++v) {
            int vx, vy;
            get_coords(v, vx, vy);
            // If node v is in the second set (odd sum) and is matched
            if ((vx + vy) % 2 != 0 && matchR[v] != -1) {
                int u = matchR[v];
                int x1, y1, x2, y2;
                get_coords(u, x1, y1);
                get_coords(v, x2, y2);
                if (y1 == y2) {
                    // Horizontal: same row, different column
                    horiz.push_back({min(x1, x2), y1});
                } else {
                    // Vertical: same column, different row
                    vert.push_back({x1, min(y1, y2)});
                }
            }
        }

        // Output the horizontal dominoes
        cout << horiz.size() << "\n";
        for (const auto& d : horiz) {
            cout << d.x << " " << d.y << "\n";
        }
        // Output the vertical dominoes
        cout << vert.size() << "\n";
        for (const auto& d : vert) {
            cout << d.x << " " << d.y << "\n";
        }
    }

    return 0;
}

