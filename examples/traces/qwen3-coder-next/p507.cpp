#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> children(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        children[p].push_back(i);
    }

    vector<long long> leaf_values(n + 1, 0);
    for (int i = n - m + 1; i <= n; i++) {
        cin >> leaf_values[i];
    }

    vector<int> order;
    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : children[u]) {
            q.push(v);
        }
    }
    reverse(order.begin(), order.end());

    vector<vector<long long>> sets(n + 1);
    vector<long long> ans(n + 1, 0);

    for (int u : order) {
        if (children[u].empty()) {
            sets[u].push_back(leaf_values[u]);
        } else {
            int idx = -1;
            for (int v : children[u]) {
                if (idx == -1 || sets[v].size() > sets[idx].size()) {
                    idx = v;
                }
            }
            sets[u] = move(sets[idx]);
            for (int v : children[u]) {
                if (v == idx) continue;
                vector<long long> temp;
                temp.reserve(sets[u].size() + sets[v].size());
                merge(sets[u].begin(), sets[u].end(), sets[v].begin(), sets[v].end(), back_inserter(temp));
                sets[u] = move(temp);
            }
            if (sets[u].size() == 1) {
                ans[u] = 2147483647;
            } else {
                long long min_diff = (1LL << 60);
                for (int i = 0; i < (int)sets[u].size() - 1; i++) {
                    long long diff = sets[u][i + 1] - sets[u][i];
                    if (diff < min_diff) {
                        min_diff = diff;
                    }
                }
                ans[u] = min_diff;
            }
        }
    }

    for (int i = 1; i <= n - m; i++) {
        cout << ans[i];
        if (i < n - m) {
            cout << ' ';
        } else {
            cout << '\n';
        }
    }

    return 0;
}
