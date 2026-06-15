#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int q, c, n, t;
    cin >> q >> c >> n >> t;

    vector<int> xs(n+1), ys(n+1);
    for (int i = 1; i <= n; i++) {
        cin >> xs[i] >> ys[i];
    }

    // Initialize dist and best_idx
    vector<vector<int>> dist(q+1, vector<int>(c+1, -1));
    vector<vector<int>> best_idx(q+1, vector<int>(c+1, 1000000));
    vector<vector<bool>> visited(q+1, vector<bool>(c+1, false));

    vector<pair<int, int>> current;
    for (int i = 1; i <= n; i++) {
        int x = xs[i];
        int y = ys[i];
        dist[x][y] = 0;
        best_idx[x][y] = i;
        visited[x][y] = true;
        current.push_back({x, y});
    }

    const int dx[4] = {0, 0, 1, -1};
    const int dy[4] = {1, -1, 0, 0};

    while (!current.empty()) {
        map<pair<int, int>, int> next_map;
        for (auto &cell : current) {
            int x = cell.first;
            int y = cell.second;
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx >= 1 && nx <= q && ny >= 1 && ny <= c) {
                    if (!visited[nx][ny]) {
                        pair<int, int> p = make_pair(nx, ny);
                        if (next_map.find(p) == next_map.end()) {
                            next_map[p] = best_idx[x][y];
                        } else {
                            if (best_idx[x][y] < next_map[p]) {
                                next_map[p] = best_idx[x][y];
                            }
                        }
                    }
                }
            }
        }

        vector<pair<int, int>> next_layer;
        for (auto &kv : next_map) {
            int nx = kv.first.first;
            int ny = kv.first.second;
            int idx_val = kv.second;
            dist[nx][ny] = dist[current[0].first][current[0].second] + 1;
            best_idx[nx][ny] = idx_val;
            visited[nx][ny] = true;
            next_layer.push_back(kv.first);
        }
        current = next_layer;
    }

    vector<int> ans(n+1, 0);
    for (int x = 1; x <= q; x++) {
        for (int y = 1; y <= c; y++) {
            if (dist[x][y] <= t) {
                ans[best_idx[x][y]]++;
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        cout << ans[i] << '\n';
    }

    return 0;
}
