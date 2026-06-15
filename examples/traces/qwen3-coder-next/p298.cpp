#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int a, b, c;
    Edge(int a, int b, int c) : a(a), b(b), c(c) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    vector<Edge> edges;
    for (int i = 1; i <= N; i++) {
        edges.emplace_back(0, i, 10000);
        edges.emplace_back(i, 0, 10000);
    }

    for (int i = 0; i < M; i++) {
        int Xi, Yi, Ci;
        cin >> Xi >> Yi >> Ci;
        edges.emplace_back(Xi, Yi, -Ci);
    }

    vector<long long> dist(N + 1, LLONG_MAX);
    dist[0] = 0;

    for (int i = 0; i <= N; i++) {
        bool updated = false;
        for (const auto& e : edges) {
            if (dist[e.a] != LLONG_MAX && dist[e.a] + e.c < dist[e.b]) {
                dist[e.b] = dist[e.a] + e.c;
                updated = true;
            }
        }
        if (!updated) break;
    }

    for (const auto& e : edges) {
        if (dist[e.a] != LLONG_MAX && dist[e.a] + e.c < dist[e.b]) {
            cout << -1 << '\n';
            return 0;
        }
    }

    for (int i = 1; i <= N; i++) {
        cout << dist[i];
        if (i < N) 
            cout << ' ';
        else 
            cout << '\n';
    }

    return 0;
}
