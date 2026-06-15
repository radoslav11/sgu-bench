#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<string> grid(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
    }

    vector<vector<pair<int, int>>> craters;
    vector<vector<bool>> vis(N, vector<bool>(M, false));
    vector<bool> in_E;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] != '.' && !vis[i][j]) {
                vector<pair<int, int>> comp;
                queue<pair<int, int>> q;
                q.push({i, j});
                vis[i][j] = true;
                bool is_E = false;
                if (i == 0 || i == N - 1 || j == 0 || j == M - 1) is_E = true;
                
                while (!q.empty()) {
                    auto [r, c] = q.front(); q.pop();
                    comp.push_back({r, c});
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + dr[k], nc = c + dc[k];
                        if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] != '.' && !vis[nr][nc]) {
                            vis[nr][nc] = true;
                            if (nr == 0 || nr == N - 1 || nc == 0 || nc == M - 1) is_E = true;
                            q.push({nr, nc});
                        }
                    }
                }
                craters.push_back(comp);
                in_E.push_back(is_E);
            }
        }
    }

    int num_E = 0;
    for (bool e : in_E) if (e) num_E++;

    if (num_E >= 2) {
        for (int i = 0; i < N; ++i) cout << grid[i] << "\n";
        return 0;
    }

    int C = craters.size();
    vector<vector<vector<int>>> dist_C(C, vector<vector<int>>(N, vector<int>(M, INF)));
    vector<vector<vector<pair<int, int>>>> parent_C(C, vector<vector<pair<int, int>>>(N, vector<pair<int, int>>(M, {-1, -1})));

    for (int i = 0; i < C; ++i) {
        if (in_E[i]) continue;
        queue<pair<int, int>> q;
        for (auto& cell : craters[i]) {
            dist_C[i][cell.first][cell.second] = 0;
            parent_C[i][cell.first][cell.second] = cell;
            q.push(cell);
        }
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (int k = 0; k < 4; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] == '.') {
                    if (dist_C[i][nr][nc] > dist_C[i][r][c] + 1) {
                        dist_C[i][nr][nc] = dist_C[i][r][c] + 1;
                        parent_C[i][nr][nc] = {r, c};
                        q.push({nr, nc});
                    }
                }
            }
        }
    }

    vector<vector<int>> distO(N, vector<int>(M, INF));
    vector<vector<pair<int, int>>> parentO(N, vector<pair<int, int>>(M, {-1, -1}));
    queue<pair<int, int>> qO;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if ((i == 0 || i == N - 1 || j == 0 || j == M - 1) && grid[i][j] == '.') {
                distO[i][j] = 1;
                parentO[i][j] = {i, j};
                qO.push({i, j});
            }
        }
    }

    while (!qO.empty()) {
        auto [r, c] = qO.front(); qO.pop();
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] == '.') {
                if (distO[nr][nc] > distO[r][c] + 1) {
                    distO[nr][nc] = distO[r][c] + 1;
                    parentO[nr][nc] = {r, c};
                    qO.push({nr, nc});
                }
            }
        }
    }

    auto cost_to_ocean = [&](int ci, int r, int c) {
        if (dist_C[ci][r][c] == INF) return INF;
        bool is_boundary = (r == 0 || r == N - 1 || c == 0 || c == M - 1);
        if (is_boundary) {
            return dist_C[ci][r][c];
        } else {
            if (distO[r][c] == INF) return INF;
            return dist_C[ci][r][c] + distO[r][c];
        }
    };

    if (num_E == 1) {
        int best_ci = -1;
        int min_D = INF;
        pair<int, int> best_J = {-1, -1};
        
        for (int i = 0; i < C; ++i) {
            if (in_E[i]) continue;
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < M; ++c) {
                    if (grid[r][c] != '.') continue;
                    int cst = cost_to_ocean(i, r, c);
                    if (cst < min_D) {
                        min_D = cst;
                        best_ci = i;
                        best_J = {r, c};
                    }
                }
            }
        }
        
        int r = best_J.first, c = best_J.second;
        while (r != -1) {
            if (grid[r][c] == '.') grid[r][c] = '*';
            if (parent_C[best_ci][r][c].first == r && parent_C[best_ci][r][c].second == c) break;
            int nr = parent_C[best_ci][r][c].first;
            int nc = parent_C[best_ci][r][c].second;
            r = nr; c = nc;
        }
        
        bool is_boundary = (best_J.first == 0 || best_J.first == N - 1 || best_J.second == 0 || best_J.second == M - 1);
        if (!is_boundary) {
            int r2 = best_J.first, c2 = best_J.second;
            while (r2 != -1) {
                if (grid[r2][c2] == '.') grid[r2][c2] = '*';
                if (parentO[r2][c2].first == r2 && parentO[r2][c2].second == c2) break;
                int nr = parentO[r2][c2].first;
                int nc = parentO[r2][c2].second;
                r2 = nr; c2 = nc;
            }
        }
    } else {
        vector<pair<int, pair<int, pair<int, int>>>> crater_D;
        for (int i = 0; i < C; ++i) {
            if (in_E[i]) continue;
            int min_D = INF;
            pair<int, int> best_J = {-1, -1};
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < M; ++c) {
                    if (grid[r][c] != '.') continue;
                    int cst = cost_to_ocean(i, r, c);
                    if (cst < min_D) {
                        min_D = cst;
                        best_J = {r, c};
                    }
                }
            }
            crater_D.push_back({min_D, {i, best_J}});
        }
        sort(crater_D.begin(), crater_D.end());
        
        int opt1_cost = INF;
        if (crater_D.size() >= 2) {
            opt1_cost = crater_D[0].first + crater_D[1].first;
        }
        
        int opt2_cost = INF;
        int best_shared_J_r = -1, best_shared_J_c = -1;
        int best_c1_shared = -1, best_c2_shared = -1;
        
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < M; ++c) {
                if (grid[r][c] != '.') continue;
                
                int min1 = INF, min2 = INF;
                int c1 = -1, c2 = -1;
                for (int i = 0; i < C; ++i) {
                    if (in_E[i]) continue;
                    if (dist_C[i][r][c] < min1) {
                        min2 = min1; c2 = c1;
                        min1 = dist_C[i][r][c]; c1 = i;
                    } else if (dist_C[i][r][c] < min2) {
                        min2 = dist_C[i][r][c]; c2 = i;
                    }
                }
                
                if (c1 != -1 && c2 != -1) {
                    bool is_boundary = (r == 0 || r == N - 1 || c == 0 || c == M - 1);
                    int cst = min1 + min2 + (is_boundary ? 0 : distO[r][c]);
                    if (cst < opt2_cost) {
                        opt2_cost = cst;
                        best_shared_J_r = r;
                        best_shared_J_c = c;
                        best_c1_shared = c1;
                        best_c2_shared = c2;
                    }
                }
            }
        }
        
        if (opt1_cost <= opt2_cost) {
            for (int k = 0; k < 2; ++k) {
                int ci = crater_D[k].second.first;
                int r = crater_D[k].second.second.first;
                int c = crater_D[k].second.second.second;
                
                int curr_r = r, curr_c = c;
                while (curr_r != -1) {
                    if (grid[curr_r][curr_c] == '.') grid[curr_r][curr_c] = '*';
                    if (parent_C[ci][curr_r][curr_c].first == curr_r && parent_C[ci][curr_r][curr_c].second == curr_c) break;
                    int nr = parent_C[ci][curr_r][curr_c].first;
                    int nc = parent_C[ci][curr_r][curr_c].second;
                    curr_r = nr; curr_c = nc;
                }
                
                bool is_boundary = (r == 0 || r == N - 1 || c == 0 || c == M - 1);
                if (!is_boundary) {
                    int r2 = r, c2 = c;
                    while (r2 != -1) {
                        if (grid[r2][c2] == '.') grid[r2][c2] = '*';
                        if (parentO[r2][c2].first == r2 && parentO[r2][c2].second == c2) break;
                        int nr = parentO[r2][c2].first;
                        int nc = parentO[r2][c2].second;
                        r2 = nr; c2 = nc;
                    }
                }
            }
        } else {
            int r = best_shared_J_r;
            int c = best_shared_J_c;
            
            for (int k = 0; k < 2; ++k) {
                int ci = (k == 0) ? best_c1_shared : best_c2_shared;
                int curr_r = r, curr_c = c;
                while (curr_r != -1) {
                    if (grid[curr_r][curr_c] == '.') grid[curr_r][curr_c] = '*';
                    if (parent_C[ci][curr_r][curr_c].first == curr_r && parent_C[ci][curr_r][curr_c].second == curr_c) break;
                    int nr = parent_C[ci][curr_r][curr_c].first;
                    int nc = parent_C[ci][curr_r][curr_c].second;
                    curr_r = nr; curr_c = nc;
                }
            }
            
            bool is_boundary = (r == 0 || r == N - 1 || c == 0 || c == M - 1);
            if (!is_boundary) {
                int r2 = r, c2 = c;
                while (r2 != -1) {
                    if (grid[r2][c2] == '.') grid[r2][c2] = '*';
                    if (parentO[r2][c2].first == r2 && parentO[r2][c2].second == c2) break;
                    int nr = parentO[r2][c2].first;
                    int nc = parentO[r2][c2].second;
                    r2 = nr; c2 = nc;
                }
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << grid[i] << "\n";
    }

    return 0;
}
