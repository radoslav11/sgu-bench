#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * The goal is to determine if team 1 can end up with at least as many wins as any other team.
 * 1. Assume team 1 wins all its remaining r1 games, so its maximum wins W1 = w1 + r1.
 * 2. For any other team i (i > 1), its current wins wi must be <= W1. If not, team 1 can't finish first.
 * 3. For all other teams i, j (i, j > 1), there are a_ij games remaining between them.
 *    Each of these games must result in a win for either i or j.
 * 4. To minimize the final wins of team i, we assume all its remaining games against teams
 *    outside the division and against team 1 are losses.
 * 5. The only wins that must be distributed among teams 2, ..., N are the a_ij games.
 * 6. We model this as a max-flow problem:
 *    - Source S connects to each pair node (i, j) with capacity a_ij.
 *    - Each pair node (i, j) connects to team nodes i and j with infinite capacity.
 *    - Each team node i (i > 1) connects to sink T with capacity (W1 - w[i]).
 *    - If the max flow equals the sum of all a_ij (for 1 < i < j <= N), then it's possible.
 */

const long long INF = 1e18;

struct Edge {
    int to;
    long long capacity;
    int rev;
};

vector<vector<Edge>> adj;
vector<int> level;
vector<int> iter;

void add_edge(int from, int to, long long capacity) {
    adj[from].push_back({to, capacity, (int)adj[to].size()});
    adj[to].push_back({from, 0, (int)adj[from].size() - 1});
}

bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    queue<int> que;
    que.push(s);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (auto& e : adj[v]) {
            if (e.capacity > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
    return level[t] != -1;
}

long long dfs(int v, int t, long long f) {
    if (v == t) return f;
    for (int& i = iter[v]; i < (int)adj[v].size(); ++i) {
        Edge& e = adj[v][i];
        if (e.capacity > 0 && level[v] < level[e.to]) {
            long long d = dfs(e.to, t, min(f, e.capacity));
            if (d > 0) {
                e.capacity -= d;
                adj[e.to][e.rev].capacity += d;
                return d;
            }
        }
    }
    return 0;
}

long long max_flow(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
        fill(iter.begin(), iter.end(), 0);
        long long f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
    return flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<int> w(N), r(N);
    for (int i = 0; i < N; ++i) cin >> w[i];
    for (int i = 0; i < N; ++i) cin >> r[i];

    vector<vector<int>> a(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> a[i][j];
        }
    }

    // W1 is team 1's max possible wins.
    int W1 = w[0] + r[0];
    for (int i = 1; i < N; ++i) {
        if (w[i] > W1) {
            cout << "NO" << endl;
            return 0;
        }
    }

    // Modeling as max flow
    int S = 0, T = 1;
    int pair_node = 2;
    // team_node_base to provide distinct nodes for team nodes 2 to N.
    int team_node_base = 2 + (N * N); 
    int total_nodes = 2 + (N * N) + (N + 1);

    adj.assign(total_nodes, vector<Edge>());
    level.assign(total_nodes, -1);
    iter.assign(total_nodes, 0);

    long long total_a_ij = 0;
    for (int i = 1; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (a[i][j] > 0) {
                total_a_ij += a[i][j];
                add_edge(S, pair_node, a[i][j]);
                add_edge(pair_node, team_node_base + i, INF);
                add_edge(pair_node, team_node_base + j, INF);
                pair_node++;
            }
        }
    }

    // Team nodes to Sink T
    for (int i = 1; i < N; ++i) {
        add_edge(team_node_base + i, T, (long long)W1 - w[i]);
    }

    if (max_flow(S, T) == total_a_ij) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}

