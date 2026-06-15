#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 100010;

vector<int> g[MAXN];
int dist[3][MAXN];

void bfs(int s, int *d, int n) {
    queue<int> q;
    memset(d, -1, sizeof(int) * (n + 1));
    d[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (d[v] == -1) {
                d[v] = d[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    bfs(1, dist[0], n);
    bfs(2, dist[1], n);
    bfs(3, dist[2], n);

    int ans = 2;
    for (int i = 1; i <= n; i++) {
        if (dist[0][i] <= dist[1][i] && dist[0][i] <= dist[2][i]) {
            if (dist[1][i] <= dist[0][i] + dist[2][i]) {
                ans = 1;
            }
        }
        if (dist[1][i] <= dist[0][i] && dist[1][i] <= dist[2][i]) {
            if (dist[0][i] <= dist[1][i] + dist[2][i]) {
                ans = 1;
            }
        }
        if (dist[2][i] <= dist[0][i] && dist[2][i] <= dist[1][i]) {
            if (dist[0][i] <= dist[1][i] + dist[2][i]) {
                ans = 1;
            }
        }
    }

    cout << ans << endl;
    return 0;
}
