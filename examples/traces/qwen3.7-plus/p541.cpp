#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n + 1);
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        deg[u]++;
        deg[v]++;
    }

    vector<int> V4;
    vector<bool> is_V4(n + 1, false);
    for (int i = 1; i <= n; i++) {
        if (deg[i] == 4) {
            V4.push_back(i);
            is_V4[i] = true;
        }
    }

    vector<vector<pair<int, int>>> multigraph_edges(n + 1);
    for (int v : V4) {
        for (int u : adj[v]) {
            int curr = u;
            int prev = v;
            int len = 1;
            while (!is_V4[curr]) {
                int next_node = -1;
                for (int nxt : adj[curr]) {
                    if (nxt != prev) {
                        next_node = nxt;
                        break;
                    }
                }
                prev = curr;
                curr = next_node;
                len++;
            }
            multigraph_edges[v].push_back({curr, len});
        }
    }

    vector<int> cycle_v;
    int curr_v = V4[0];
    int prev_v = -1;
    cycle_v.push_back(curr_v);
    
    while (true) {
        int next_v = -1;
        for (auto& edge : multigraph_edges[curr_v]) {
            if (edge.first != prev_v) {
                next_v = edge.first;
                break;
            }
        }
        if (next_v == V4[0] && cycle_v.size() >= 3) {
            break;
        }
        prev_v = curr_v;
        curr_v = next_v;
        cycle_v.push_back(curr_v);
    }

    int k = cycle_v.size();
    vector<int> odd(k, 0);
    for (int i = 0; i < k; i++) {
        int u = cycle_v[i];
        int w = cycle_v[(i + 1) % k];
        int sum_len = 0;
        for (auto& edge : multigraph_edges[u]) {
            if (edge.first == w) {
                sum_len += edge.second;
            }
        }
        if (sum_len % 2 == 1) {
            odd[i] = 1;
        }
    }

    auto solve = [&](int fixed_state) {
        vector<vector<int>> dp(k, vector<int>(2, INF));
        vector<vector<int>> parent(k, vector<int>(2, -1));
        
        if (fixed_state == 0) {
            dp[0][0] = 0;
        } else {
            dp[0][1] = 1;
        }
        
        for (int i = 1; i < k; i++) {
            for (int s = 0; s < 2; s++) {
                for (int prev_s = 0; prev_s < 2; prev_s++) {
                    if (dp[i-1][prev_s] == INF) continue;
                    if (odd[i-1] == 1 && prev_s == 0 && s == 0) continue;
                    
                    int cost = dp[i-1][prev_s] + s;
                    if (cost < dp[i][s]) {
                        dp[i][s] = cost;
                        parent[i][s] = prev_s;
                    }
                }
            }
        }
        
        int best_ans = INF;
        int best_s = -1;
        for (int s = 0; s < 2; s++) {
            if (dp[k-1][s] == INF) continue;
            if (odd[k-1] == 1 && s == 0 && fixed_state == 0) continue;
            
            if (dp[k-1][s] < best_ans) {
                best_ans = dp[k-1][s];
                best_s = s;
            }
        }
        
        if (best_ans == INF) return make_pair(INF, vector<int>());
        
        vector<int> res;
        int curr_s = best_s;
        if (curr_s == 1) res.push_back(k - 1);
        for (int i = k - 1; i >= 1; i--) {
            curr_s = parent[i][curr_s];
            if (curr_s == 1) res.push_back(i - 1);
        }
        return make_pair(best_ans, res);
    };

    auto ans0 = solve(0);
    auto ans1 = solve(1);

    vector<int> best_res;
    if (ans0.first < ans1.first) {
        best_res = ans0.second;
    } else {
        best_res = ans1.second;
    }

    vector<bool> removed(n + 1, false);
    for (int idx : best_res) {
        removed[cycle_v[idx]] = true;
    }

    vector<int> comp1, comp2;
    vector<int> color(n + 1, -1);

    for (int i = 1; i <= n; i++) {
        if (removed[i]) continue;
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                if (color[u] == 0) comp1.push_back(u);
                else comp2.push_back(u);
                
                for (int v : adj[u]) {
                    if (removed[v]) continue;
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    }
                }
            }
        }
    }

    cout << comp1.size() << "\n";
    for (size_t i = 0; i < comp1.size(); i++) {
        cout << comp1[i] << (i + 1 == comp1.size() ? "" : " ");
    }
    cout << "\n";

    cout << comp2.size() << "\n";
    for (size_t i = 0; i < comp2.size(); i++) {
        cout << comp2[i] << (i + 1 == comp2.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
