#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    vector<int> depth(n + 1, -1);
    vector<int> parent(n + 1, -1);
    vector<vector<int>> cycles;
    vector<vector<int>> cycle_id(n + 1);

    depth[1] = 0;
    parent[1] = -1;
    vector<int> stack;
    stack.push_back(1);
    int cnt = 0;

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int v : g[u]) {
            if (v == parent[u]) continue;
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                parent[v] = u;
                stack.push_back(v);
            } else if (depth[v] < depth[u]) {
                vector<int> cycle;
                int cur = u;
                while (cur != v) {
                    cycle.push_back(cur);
                    cur = parent[cur];
                }
                cycle.push_back(v);
                cycles.push_back(cycle);
                int cycle_index = cycles.size() - 1;
                for (int x : cycle) {
                    cycle_id[x].push_back(cycle_index);
                }
            }
        }
    }

    vector<int> remove_set;
    vector<bool> cycle_covered(cycles.size(), false);

    for (int v = 1; v <= n; v++) {
        if ((int)cycle_id[v].size() == 2) {
            int c1 = cycle_id[v][0];
            int c2 = cycle_id[v][1];
            if (!cycle_covered[c1] && !cycle_covered[c2] && 
                (int)cycles[c1].size() % 2 == 1 && 
                (int)cycles[c2].size() % 2 == 1) {
                remove_set.push_back(v);
                cycle_covered[c1] = true;
                cycle_covered[c2] = true;
            }
        }
    }

    for (int i = 0; i < (int)cycles.size(); i++) {
        if (!cycle_covered[i] && (int)cycles[i].size() % 2 == 1) {
            remove_set.push_back(cycles[i][0]);
            cycle_covered[i] = true;
        }
    }

    vector<bool> in_remove_set(n + 1, false);
    for (int v : remove_set) {
        in_remove_set[v] = true;
    }

    vector<vector<int>> new_g(n + 1);
    for (int u = 1; u <= n; u++) {
        if (in_remove_set[u]) continue;
        for (int v : g[u]) {
            if (!in_remove_set[v]) {
                new_g[u].push_back(v);
            }
        }
    }

    vector<int> color(n + 1, -1);
    vector<int> part0, part1;

    for (int i = 1; i <= n; i++) {
        if (!in_remove_set[i] && color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;
            part0.push_back(i);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : new_g[u]) {
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        if (color[v] == 0) {
                            part0.push_back(v);
                        } else {
                            part1.push_back(v);
                        }
                        q.push(v);
                    }
                }
            }
        }
    }

    cout << part0.size();
    for (int i = 0; i < (int)part0.size(); i++) {
        cout << " " << part0[i];
    }
    cout << "\n";

    cout << part1.size();
    for (int i = 0; i < (int)part1.size(); i++) {
        cout << " " << part1[i];
    }
    cout << "\n";

    return 0;
}
