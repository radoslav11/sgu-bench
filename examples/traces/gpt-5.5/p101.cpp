#include <bits/stdc++.h>
using namespace std;

struct AdjEdge {
    int to, id;
};

struct UsedEdge {
    int id, from, to;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> a(N + 1), b(N + 1), deg(7, 0);
    vector<vector<AdjEdge>> g(7);

    for (int i = 1; i <= N; i++) {
        cin >> a[i] >> b[i];

        deg[a[i]]++;
        deg[b[i]]++;

        g[a[i]].push_back({b[i], i});
        g[b[i]].push_back({a[i], i});
    }

    int oddCount = 0;
    int start = -1;

    for (int i = 0; i <= 6; i++) {
        if (deg[i] % 2 == 1) {
            oddCount++;
            start = i;
        }
    }

    if (oddCount != 0 && oddCount != 2) {
        cout << "No solution\n";
        return 0;
    }

    if (start == -1) {
        for (int i = 0; i <= 6; i++) {
            if (deg[i] > 0) {
                start = i;
                break;
            }
        }
    }

    vector<int> visited(7, 0);
    queue<int> q;
    q.push(start);
    visited[start] = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto e : g[v]) {
            if (!visited[e.to]) {
                visited[e.to] = 1;
                q.push(e.to);
            }
        }
    }

    for (int i = 0; i <= 6; i++) {
        if (deg[i] > 0 && !visited[i]) {
            cout << "No solution\n";
            return 0;
        }
    }

    vector<int> used(N + 1, 0);
    vector<UsedEdge> answer;

    function<void(int)> dfs = [&](int v) {
        while (!g[v].empty()) {
            AdjEdge e = g[v].back();
            g[v].pop_back();

            if (used[e.id]) continue;

            used[e.id] = 1;
            dfs(e.to);
            answer.push_back({e.id, v, e.to});
        }
    };

    dfs(start);

    if ((int)answer.size() != N) {
        cout << "No solution\n";
        return 0;
    }

    reverse(answer.begin(), answer.end());

    for (auto e : answer) {
        char sign;
        if (a[e.id] == e.from && b[e.id] == e.to) {
            sign = '+';
        } else {
            sign = '-';
        }

        cout << e.id << ' ' << sign << '\n';
    }

    return 0;
}
