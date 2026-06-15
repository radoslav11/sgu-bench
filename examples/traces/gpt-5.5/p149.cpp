#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10000 + 5;
const int MAXE = 2 * MAXN;

int n;
int head[MAXN], to[MAXE], nxt[MAXE], weight_[MAXE], ecnt = 0;
long long distA[MAXN], distB[MAXN];

void addEdge(int u, int v, int w) {
    to[ecnt] = v;
    weight_[ecnt] = w;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

int farthestFrom(int src, long long dist[]) {
    static int stNode[MAXN], stPar[MAXN];
    int top = 0;
    stNode[top] = src;
    stPar[top] = 0;
    dist[src] = 0;

    int far = src;

    while (top >= 0) {
        int u = stNode[top];
        int p = stPar[top];
        top--;

        if (dist[u] > dist[far]) far = u;

        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (v == p) continue;
            dist[v] = dist[u] + weight_[e];
            top++;
            stNode[top] = v;
            stPar[top] = u;
        }
    }

    return far;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    fill(head, head + n + 1, -1);

    for (int i = 2; i <= n; i++) {
        int parent, len;
        cin >> parent >> len;
        addEdge(i, parent, len);
        addEdge(parent, i, len);
    }

    int a = farthestFrom(1, distA);
    int b = farthestFrom(a, distA);
    farthestFrom(b, distB);

    for (int i = 1; i <= n; i++) {
        cout << max(distA[i], distB[i]) << '\n';
    }

    return 0;
}
