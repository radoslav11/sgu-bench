#include <bits/stdc++.h>
using namespace std;

int N, M;
struct Edge { int a, b, c, t; };
vector<Edge> edges;

vector<int> findPosCycle(double lambda) {
    vector<double> dist(N+1, 0.0);
    vector<int> par(N+1, -1);
    int x = -1;
    for (int iter = 0; iter < N+1; iter++) {
        x = -1;
        for (int e = 0; e < M; e++) {
            int a = edges[e].a, b = edges[e].b;
            double w = edges[e].c - lambda * edges[e].t;
            if (dist[a] + w > dist[b] + 1e-9) {
                dist[b] = dist[a] + w;
                par[b] = a;
                x = b;
            }
        }
        if (x == -1) break;
    }
    if (x == -1) return {};
    for (int i = 0; i < N; i++) x = par[x];
    vector<int> cyc;
    int cur = x;
    do {
        cyc.push_back(cur);
        cur = par[cur];
    } while (cur != x);
    reverse(cyc.begin(), cyc.end());
    return cyc;
}

int main(){
    scanf("%d %d", &N, &M);
    for (int i = 0; i < M; i++) {
        int a,b,c,t;
        scanf("%d %d %d %d", &a,&b,&c,&t);
        edges.push_back({a,b,c,t});
    }
    if (findPosCycle(0.0).empty()) { printf("0\n"); return 0; }
    double lo = 0.0, hi = 101.0;
    for (int it = 0; it < 100; it++) {
        double mid = (lo + hi) / 2;
        if (!findPosCycle(mid).empty()) lo = mid; else hi = mid;
    }
    vector<int> cyc;
    for (double d = 1e-6; d < 2.0 && cyc.empty(); d *= 2) cyc = findPosCycle(lo - d);
    if (cyc.empty()) { printf("0\n"); return 0; }
    printf("%d\n", (int)cyc.size());
    for (int i = 0; i < (int)cyc.size(); i++)
        printf("%d%c", cyc[i], i+1<(int)cyc.size()?' ':'\n');
    return 0;
}
