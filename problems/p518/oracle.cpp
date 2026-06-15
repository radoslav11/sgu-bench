#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<int>> g(n, vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> g[i][j];
        }
    }

    int k;
    cin >> k;
    vector<int> r(k);
    for(int i = 0; i < k; i++) {
        cin >> r[i];
    }

    // BFS over states (node, matched). From (u, p) with p < k we may move to
    // any v with g[u][v] == r[p], reaching (v, p+1). States with p == k mark
    // possible end nodes.
    vector<vector<char>> vis(n, vector<char>(k + 1, 0));
    deque<pair<int, int>> q;
    vis[0][0] = 1;
    q.push_back({0, 0});
    set<int> possible;

    while(!q.empty()) {
        auto [u, p] = q.front();
        q.pop_front();
        if(p == k) {
            possible.insert(u);
            continue;
        }
        for(int v = 0; v < n; v++) {
            if(g[u][v] == r[p] && !vis[v][p + 1]) {
                vis[v][p + 1] = 1;
                q.push_back({v, p + 1});
            }
        }
    }

    cout << possible.size() << "\n";
    if(!possible.empty()) {
        bool first = true;
        for(int x: possible) {
            cout << (first ? "" : " ") << (x + 1);
            first = false;
        }
        cout << "\n";
    }
    return 0;
}
