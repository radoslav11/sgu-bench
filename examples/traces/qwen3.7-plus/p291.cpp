#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int owner;
    int x, y;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int q, c, n;
    long long t;
    if (!(cin >> q >> c >> n >> t)) return 0;

    int cols = c + 1;
    vector<int> owner((q + 1) * cols, 0);
    vector<int> dist((q + 1) * cols, 1e9);

    int limit = min((long long)(q + c), t);
    vector<vector<Item>> level_q(limit + 1);

    for (int i = 1; i <= n; ++i) {
        int x, y;
        cin >> x >> y;
        owner[x * cols + y] = i;
        dist[x * cols + y] = 0;
        level_q[0].push_back({i, x, y});
    }

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int d = 0; d < limit; ++d) {
        if (level_q[d].empty()) continue;

        sort(level_q[d].begin(), level_q[d].end(), [](const Item& a, const Item& b) {
            return a.owner < b.owner;
        });

        for (const auto& item : level_q[d]) {
            int x = item.x;
            int y = item.y;
            int curr_owner = item.owner;

            for (int dir = 0; dir < 4; ++dir) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];

                if (nx >= 1 && nx <= q && ny >= 1 && ny <= c) {
                    int idx = nx * cols + ny;
                    if (dist[idx] > d + 1) {
                        dist[idx] = d + 1;
                        owner[idx] = curr_owner;
                        level_q[d + 1].push_back({curr_owner, nx, ny});
                    }
                }
            }
        }
    }

    vector<int> counts(n + 1, 0);
    for (int x = 1; x <= q; ++x) {
        for (int y = 1; y <= c; ++y) {
            if (dist[x * cols + y] <= t) {
                counts[owner[x * cols + y]]++;
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        cout << counts[i] << "\n";
    }

    return 0;
}
