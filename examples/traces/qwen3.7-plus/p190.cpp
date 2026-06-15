#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, P;
    if (!(cin >> N >> P)) return 0;

    vector<vector<bool>> valid(N + 1, vector<bool>(N + 1, true));
    for (int i = 0; i < P; ++i) {
        int x, y;
        cin >> x >> y;
        valid[x][y] = false;
    }

    int valid_count = 0;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (valid[i][j]) {
                valid_count++;
            }
        }
    }

    if (valid_count % 2 != 0) {
        cout << "No\n";
        return 0;
    }

    int u_cnt = 0, v_cnt = 0;
    vector<vector<int>> U_map(N + 1, vector<int>(N + 1, -1));
    vector<vector<int>> V_map(N + 1, vector<int>(N + 1, -1));
    vector<pair<int, int>> U_to_coord;
    vector<pair<int, int>> V_to_coord;

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (!valid[i][j]) continue;
            if ((i + j) % 2 == 0) {
                U_map[i][j] = u_cnt++;
                U_to_coord.push_back({i, j});
            } else {
                V_map[i][j] = v_cnt++;
                V_to_coord.push_back({i, j});
            }
        }
    }

    if (u_cnt != v_cnt) {
        cout << "No\n";
        return 0;
    }

    vector<vector<int>> adj(u_cnt);
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (!valid[i][j] || (i + j) % 2 != 0) continue;
            int u = U_map[i][j];
            for (int k = 0; k < 4; ++k) {
                int nx = i + dx[k];
                int ny = j + dy[k];
                if (nx >= 1 && nx <= N && ny >= 1 && ny <= N && valid[nx][ny]) {
                    adj[u].push_back(V_map[nx][ny]);
                }
            }
        }
    }

    const int INF = 1e9;
    vector<int> pairU(u_cnt, -1);
    vector<int> pairV(v_cnt, -1);
    vector<int> distU(u_cnt, 0);

    auto bfs = [&]() {
        queue<int> Q;
        for (int u = 0; u < u_cnt; ++u) {
            if (pairU[u] == -1) {
                distU[u] = 0;
                Q.push(u);
            } else {
                distU[u] = INF;
            }
        }
        bool found = false;
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            for (int v : adj[u]) {
                if (pairV[v] == -1) {
                    found = true;
                } else if (distU[pairV[v]] == INF) {
                    distU[pairV[v]] = distU[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
        return found;
    };

    auto dfs = [&](auto& self, int u) -> bool {
        for (int v : adj[u]) {
            if (pairV[v] == -1 || (distU[pairV[v]] == distU[u] + 1 && self(self, pairV[v]))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        distU[u] = INF;
        return false;
    };

    int matching = 0;
    while (bfs()) {
        for (int u = 0; u < u_cnt; ++u) {
            if (pairU[u] == -1) {
                if (dfs(dfs, u)) {
                    matching++;
                }
            }
        }
    }

    if (matching != valid_count / 2) {
        cout << "No\n";
    } else {
        cout << "Yes\n";
        vector<pair<int, int>> horiz;
        vector<pair<int, int>> vert;

        for (int u = 0; u < u_cnt; ++u) {
            int v = pairU[u];
            if (v != -1) {
                int x1 = U_to_coord[u].first;
                int y1 = U_to_coord[u].second;
                int x2 = V_to_coord[v].first;
                int y2 = V_to_coord[v].second;

                if (y1 == y2) {
                    int lx = min(x1, x2);
                    horiz.push_back({lx, y1});
                } else {
                    int by = min(y1, y2);
                    vert.push_back({x1, by});
                }
            }
        }

        cout << horiz.size() << "\n";
        for (auto p : horiz) {
            cout << p.first << " " << p.second << "\n";
        }
        cout << vert.size() << "\n";
        for (auto p : vert) {
            cout << p.first << " " << p.second << "\n";
        }
    }

    return 0;
}
