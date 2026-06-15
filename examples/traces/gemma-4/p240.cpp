#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * We need to find a path from a starting junction S to any of the E exit junctions
 * such that the maximum temperature experienced in any corridor along the path 
 * is minimized. The temperature in corridor i at time t is Ri + Pi * t.
 * The maximum temperature in a corridor occurs at the end of its traversal.
 * The temperature of the suit must not exceed H.
 * 
 * Complexity:
 * - Binary search on the maximum temperature X: log(H) iterations.
 * - For each X, use Dijkstra's algorithm to check if an exit is reachable 
 *   under the constraint that all corridor temperatures are <= X.
 * - Total complexity: O(log H * M * log N), where N <= 100, M <= 10000.
 * - This fits well within the 0.25s time limit.
 */

const int INF = 1e9;

struct Edge {
    int to, t, r, p;
};

int N, M, H, S, E;
vector<Edge> adj[105];
vector<int> exits;
int min_time[105];
int parent[105];

// Dijkstra-based check to see if any exit is reachable with maximum temperature <= X
bool check(int X) {
    for (int i = 1; i <= N; ++i) min_time[i] = INF;
    min_time[S] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, S});

    while (!pq.empty()) {
        int t = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (t > min_time[u]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int t_new = t + edge.t;
            // Use long long to avoid overflow in temperature calculation
            long long max_temp = (long long)edge.r + (long long)edge.p * t_new;
            if (max_temp <= X && t_new < min_time[v]) {
                min_time[v] = t_new;
                pq.push({min_time[v], v});
            }
        }
    }

    for (int exit_node : exits) {
        if (min_time[exit_node] != INF) return true;
    }
    return false;
}

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M >> H >> S >> E)) return 0;

    for (int i = 0; i < M; ++i) {
        int u, v, t, r, p;
        cin >> u >> v >> t >> r >> p;
        adj[u].push_back({v, t, r, p});
        adj[v].push_back({u, t, r, p});
    }

    for (int i = 0; i < E; ++i) {
        int f;
        cin >> f;
        exits.push_back(f);
    }

    // Check if it is even possible to survive
    if (!check(H)) {
        cout << "NO" << endl;
        return 0;
    }

    // Binary search for the minimal possible maximum temperature X
    int low = 0, high = H, ans = H;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (check(mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << "YES" << endl;
    cout << ans << endl;

    // One last Dijkstra to reconstruct the best path using the optimal answer ans
    for (int i = 1; i <= N; ++i) min_time[i] = INF;
    for (int i = 1; i <= N; ++i) parent[i] = 0;
    min_time[S] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, S});

    while (!pq.empty()) {
        int t = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (t > min_time[u]) continue;
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int t_new = t + edge.t;
            long long max_temp = (long long)edge.r + (long long)edge.p * t_new;
            if (max_temp <= (long long)ans && t_new < min_time[v]) {
                min_time[v] = t_new;
                parent[v] = u;
                pq.push({min_time[v], v});
            }
        }
    }

    // Pick any reachable exit node
    int best_exit = -1;
    for (int exit_node : exits) {
        if (min_time[exit_node] != INF) {
            best_exit = exit_node;
            break;
        }
    }

    // Reconstruct path using parent pointers
    vector<int> path;
    int curr = best_exit;
    while (curr != 0) {
        path.push_back(curr);
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());

    // Output path format: Z junction1 junction2 ... junctionZ
    cout << path.size();
    for (int node : path) {
        cout << " " << node;
    }
    cout << endl;

    return 0;
}

