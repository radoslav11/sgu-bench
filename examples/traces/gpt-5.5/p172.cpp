#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> g(N + 1);
    for (int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    vector<int> color(N + 1, -1);

    for (int start = 1; start <= N; ++start) {
        if (color[start] != -1) continue;

        color[start] = 0;
        queue<int> q;
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int to : g[v]) {
                if (color[to] == -1) {
                    color[to] = color[v] ^ 1;
                    q.push(to);
                } else if (color[to] == color[v]) {
                    cout << "no\n";
                    return 0;
                }
            }
        }
    }

    vector<int> firstDay;
    for (int i = 1; i <= N; ++i) {
        if (color[i] == 0) {
            firstDay.push_back(i);
        }
    }

    cout << "yes\n";
    cout << firstDay.size() << '\n';
    for (int x : firstDay) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
