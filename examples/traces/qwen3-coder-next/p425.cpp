#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> T(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> T[i][j];
        }
    }

    if (n == 1) {
        set<int> vals;
        for (int j = 0; j < m; j++) {
            vals.insert(T[0][j]);
        }
        cout << "Yes" << endl;
        for (int x : vals) {
            cout << x << " -> " << 0 << '\n';
        }
        return 0;
    }

    set<int> all_vals_set;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            all_vals_set.insert(T[i][j]);
        }
    }
    vector<int> all_vals(all_vals_set.begin(), all_vals_set.end());

    vector<vector<pair<int, int>>> row_edges;
    map<int, int> force_count;

    for (int i = 1; i < n; i++) {
        vector<pair<int, int>> edges;
        for (int j = 0; j < m; j++) {
            if (T[0][j] != T[i][j]) {
                edges.push_back({T[0][j], T[i][j]});
            }
        }
        row_edges.push_back(edges);

        if (edges.empty()) {
            continue;
        }

        set<int> cand;
        cand.insert(edges[0].first);
        cand.insert(edges[0].second);
        for (int k = 1; k < edges.size(); k++) {
            set<int> new_cand;
            for (int x : cand) {
                if (x == edges[k].first || x == edges[k].second) {
                    new_cand.insert(x);
                }
            }
            cand = new_cand;
            if (cand.empty()) {
                break;
            }
        }
        for (int x : cand) {
            force_count[x]++;
        }
    }

    bool possible = true;
    for (auto &p : force_count) {
        if (p.second > 50) {
            possible = false;
            break;
        }
    }

    if (!possible) {
        cout << "No" << endl;
        return 0;
    }

    map<int, int> deg;
    for (int x : all_vals) {
        deg[x] = 0;
    }

    vector<pair<int, int>> chosen_edges;
    for (int i = 0; i < row_edges.size(); i++) {
        vector<pair<int, int>>& edges = row_edges[i];
        int best_index = 0;
        int best_val = 1000000000;
        for (int k = 0; k < edges.size(); k++) {
            int a = edges[k].first;
            int b = edges[k].second;
            int current = max(deg[a], deg[b]);
            if (current < best_val) {
                best_val = current;
                best_index = k;
            }
        }
        int a = edges[best_index].first;
        int b = edges[best_index].second;
        chosen_edges.push_back({a, b});
        deg[a]++;
        deg[b]++;
    }

    map<int, vector<int>> adj;
    for (int x : all_vals) {
        adj[x] = vector<int>();
    }
    for (auto &e : chosen_edges) {
        int a = e.first;
        int b = e.second;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    map<int, int> f;
    for (int x : all_vals) {
        vector<bool> used(52, false);
        for (int neighbor : adj[x]) {
            if (f.find(neighbor) != f.end()) {
                if (f[neighbor] < 52) {
                    used[f[neighbor]] = true;
                }
            }
        }
        int color = 0;
        while (color < 52 && used[color]) {
            color++;
        }
        f[x] = color;
    }

    cout << "Yes" << endl;
    for (int x : all_vals) {
        cout << x << " -> " << f[x] << '\n';
    }

    return 0;
}
