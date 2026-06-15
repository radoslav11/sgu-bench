#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 30005;
const int INF = 1000000000;

vector<int> adj[MAXN];
int parent[MAXN];
int order[MAXN];
int d_un[MAXN];
int d_ce[MAXN];
int order_ptr = 0;
int N, K;

void dfs(int u, int p) {
    parent[u] = p;
    order[order_ptr++] = u;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> K)) return 0;

    if (N == 0) return 0;
    if (N == 1) {
        cout << "1\n1" << endl;
        return 0;
    }

    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, 0);

    vector<int> centers;
    for (int i = 1; i <= N; ++i) {
        d_un[i] = 0;
        d_ce[i] = INF;
    }

    for (int i = N - 1; i >= 0; --i) {
        int u = order[i];
        
        if (d_un[u] + d_ce[u] <= K) {
            d_un[u] = -INF;
        }
        
        if (d_un[u] == K) {
            centers.push_back(u);
            d_un[u] = -INF;
            d_ce[u] = 0;
        }
        
        if (i == 0 && d_un[u] >= 0) {
            centers.push_back(u);
            d_un[u] = -INF;
            d_ce[u] = 0;
        }
        
        int p = parent[u];
        if (p != 0) {
            if (d_un[u] != -INF) {
                d_un[p] = max(d_un[p], d_un[u] + 1);
            }
            d_ce[p] = min(d_ce[p], d_ce[u] + 1);
        }
    }

    cout << centers.size() << "\n";
    for (int i = 0; i < (int)centers.size(); ++i) {
        cout << centers[i] << (i == (int)centers.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

