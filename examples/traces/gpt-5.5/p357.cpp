#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d1, d2, d3, up;
    int d4, d5, d6, down;
    int d7, d8, d9;
    int dash, d0;
    int X, Y;

    if (!(cin >> d1 >> d2 >> d3 >> up)) return 0;
    cin >> d4 >> d5 >> d6 >> down;
    cin >> d7 >> d8 >> d9;
    cin >> dash >> d0;
    cin >> X >> Y;

    vector<int> digit(10);
    digit[0] = d0;
    digit[1] = d1;
    digit[2] = d2;
    digit[3] = d3;
    digit[4] = d4;
    digit[5] = d5;
    digit[6] = d6;
    digit[7] = d7;
    digit[8] = d8;
    digit[9] = d9;

    vector<pair<int, int>> direct;
    for (int ch = 0; ch < 100; ++ch) {
        if (ch < 10) {
            if (digit[ch]) direct.push_back({ch, 1});
        } else {
            int a = ch / 10;
            int b = ch % 10;
            if (dash && digit[a] && digit[b]) direct.push_back({ch, 3});
        }
    }

    const int INF = 1e9;
    vector<int> dist(100, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[X] = 0;
    pq.push({0, X});

    while (!pq.empty()) {
        auto [curDist, v] = pq.top();
        pq.pop();

        if (curDist != dist[v]) continue;

        if (up) {
            int to = (v + 1) % 100;
            if (dist[to] > curDist + 1) {
                dist[to] = curDist + 1;
                pq.push({dist[to], to});
            }
        }

        if (down) {
            int to = (v + 99) % 100;
            if (dist[to] > curDist + 1) {
                dist[to] = curDist + 1;
                pq.push({dist[to], to});
            }
        }

        for (auto [to, cost] : direct) {
            if (dist[to] > curDist + cost) {
                dist[to] = curDist + cost;
                pq.push({dist[to], to});
            }
        }
    }

    cout << (dist[Y] == INF ? -1 : dist[Y]) << '\n';

    return 0;
}
