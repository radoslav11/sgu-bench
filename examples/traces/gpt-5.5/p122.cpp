#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<vector<char>> adj(N + 1, vector<char>(N + 1, 0));
    string line;
    getline(cin, line);

    for (int i = 1; i <= N; ++i) {
        getline(cin, line);
        stringstream ss(line);
        int x;
        while (ss >> x) {
            if (x >= 1 && x <= N && x != i) {
                adj[i][x] = adj[x][i] = 1;
            }
        }
    }

    if (N == 2) {
        if (adj[1][2]) {
            cout << "1 2 1\n";
        } else {
            cout << "No solution\n";
        }
        return 0;
    }

    vector<int> deg(N + 1, 0);
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            deg[i] += adj[i][j];
        }
    }

    for (int i = 1; i <= N; ++i) {
        if (deg[i] < (N + 1) / 2) {
            cout << "No solution\n";
            return 0;
        }
    }

    vector<int> cycle(N);
    iota(cycle.begin(), cycle.end(), 1);

    while (true) {
        int bad = -1;
        for (int i = 0; i < N; ++i) {
            if (!adj[cycle[i]][cycle[(i + 1) % N]]) {
                bad = i;
                break;
            }
        }

        if (bad == -1) break;

        int a = cycle[bad];
        int b = cycle[(bad + 1) % N];

        vector<int> path;
        path.reserve(N);
        path.push_back(a);

        int cur = (bad - 1 + N) % N;
        while (cur != (bad + 1) % N) {
            path.push_back(cycle[cur]);
            cur = (cur - 1 + N) % N;
        }
        path.push_back(b);

        int pos = -1;
        for (int j = 0; j + 1 < N; ++j) {
            if (adj[b][path[j]] && adj[a][path[j + 1]]) {
                pos = j;
                break;
            }
        }

        if (pos == -1) {
            cout << "No solution\n";
            return 0;
        }

        vector<int> nextCycle;
        nextCycle.reserve(N);

        nextCycle.push_back(a);

        for (int j = pos + 1; j < N; ++j) {
            nextCycle.push_back(path[j]);
        }

        for (int j = pos; j >= 1; --j) {
            nextCycle.push_back(path[j]);
        }

        cycle = nextCycle;
    }

    int start = -1;
    for (int i = 0; i < N; ++i) {
        if (cycle[i] == 1) {
            start = i;
            break;
        }
    }

    for (int k = 0; k < N; ++k) {
        if (k) cout << ' ';
        cout << cycle[(start + k) % N];
    }
    cout << " 1\n";

    return 0;
}
