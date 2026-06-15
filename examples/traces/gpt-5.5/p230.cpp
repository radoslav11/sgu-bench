#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> g(N + 1);
    vector<int> indeg(N + 1, 0);

    for (int i = 0; i < M; ++i) {
        int P, Q;
        cin >> P >> Q;
        g[P].push_back(Q);
        indeg[Q]++;
    }

    queue<int> q;
    for (int i = 1; i <= N; ++i) {
        if (indeg[i] == 0) q.push(i);
    }

    vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);

        for (int to : g[v]) {
            indeg[to]--;
            if (indeg[to] == 0) q.push(to);
        }
    }

    if ((int)order.size() != N) {
        cout << "No solution\n";
        return 0;
    }

    vector<int> ans(N + 1);
    for (int i = 0; i < N; ++i) {
        ans[order[i]] = i + 1;
    }

    for (int i = 1; i <= N; ++i) {
        if (i > 1) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
