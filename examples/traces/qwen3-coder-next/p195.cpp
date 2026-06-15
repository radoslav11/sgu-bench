#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int NEG_INF = -1000000000;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    vector<vector<int>> children(N+1);
    if (N > 1) {
        for (int i = 0; i < N-1; i++) {
            int chief;
            cin >> chief;
            children[chief].push_back(i+2);
        }
    }

    vector<int> dp0(N+1, 0);
    vector<int> dp1(N+1, NEG_INF);
    vector<int> best_child(N+1, -1);

    for (int u = N; u >= 1; u--) {
        if (!children[u].empty()) {
            for (int v : children[u]) {
                dp0[u] += max(dp0[v], dp1[v]);
            }

            for (int v : children[u]) {
                int gain = dp0[v] - max(dp0[v], dp1[v]);
                int candidate = dp0[u] + 1 + gain;
                if (candidate > dp1[u]) {
                    dp1[u] = candidate;
                    best_child[u] = v;
                }
            }
        }
    }

    int total_matching = max(dp0[1], dp1[1]);
    cout << total_matching * 1000 << '\n';

    vector<bool> grant(N+1, false);
    vector<int> state(N+1, 0);

    if (dp1[1] > dp0[1]) {
        state[1] = 1;
    } else {
        state[1] = 0;
    }

    queue<int> q;
    q.push(1);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (state[u] == 1) {
            int v0 = best_child[u];
            grant[v0] = true;
            state[v0] = 0;
            q.push(v0);
            for (int v : children[u]) {
                if (v == v0) continue;
                if (dp0[v] >= dp1[v]) {
                    state[v] = 0;
                } else {
                    state[v] = 1;
                }
                q.push(v);
            }
        } else {
            for (int v : children[u]) {
                if (dp0[v] >= dp1[v]) {
                    state[v] = 0;
                } else {
                    state[v] = 1;
                }
                q.push(v);
            }
        }
    }

    vector<int> recipients;
    for (int i = 1; i <= N; i++) {
        if (grant[i]) {
            recipients.push_back(i);
        }
    }

    for (int i = 0; i < (int)recipients.size(); i++) {
        if (i > 0) cout << " ";
        cout << recipients[i];
    }
    cout << '\n';

    return 0;
}
