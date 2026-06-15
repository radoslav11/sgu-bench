#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <set>
#include <tuple>
using namespace std;

const int INF = 100000000;
const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> comp_id(n, vector<int>(m, -1));
    vector<vector<pair<int, int>>> components;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'X' && !visited[i][j]) {
                vector<pair<int, int>> comp;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    comp.push_back({x, y});
                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny] && grid[nx][ny] == 'X') {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
                components.push_back(comp);
                int comp_idx = components.size() - 1;
                for (auto [x, y] : comp) {
                    comp_id[x][y] = comp_idx;
                }
            }
        }
    }

    int K = components.size();
    vector<set<pair<int, int>>> adj_cells(K);

    for (int idx = 0; idx < K; idx++) {
        for (auto [x, y] : components[idx]) {
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '.') {
                    adj_cells[idx].insert({nx, ny});
                }
            }
        }
    }

    vector<vector<int>> distOcean(n, vector<int>(m, INF));
    queue<tuple<int, int, int>> q_0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i == 0 || i == n-1 || j == 0 || j == m-1) {
                if (grid[i][j] == '.') {
                    distOcean[i][j] = 1;
                    q_0.push({i, j, 1});
                }
            }
        }
    }

    while (!q_0.empty()) {
        auto [x, y, d] = q_0.front();
        q_0.pop();
        if (distOcean[x][y] < d) continue;
        for (int d_dir = 0; d_dir < 4; d_dir++) {
            int nx = x + dx[d_dir];
            int ny = y + dy[d_dir];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '.') {
                if (distOcean[nx][ny] > d + 1) {
                    distOcean[nx][ny] = d + 1;
                    q_0.push({nx, ny, d+1});
                }
            }
        }
    }

    vector<int> d_val(K, INF);
    for (int i = 0; i < K; i++) {
        for (auto [x, y] : adj_cells[i]) {
            if (distOcean[x][y] < d_val[i]) {
                d_val[i] = distOcean[x][y];
            }
        }
    }

    vector<vector<int>> dist_between(K, vector<int>(K, INF));
    for (int i = 0; i < K; i++) {
        for (int j = i+1; j < K; j++) {
            vector<vector<int>> dist_from_i(n, vector<int>(m, INF));
            queue<pair<int, int>> q;
            for (auto [x, y] : adj_cells[i]) {
                dist_from_i[x][y] = 1;
                q.push({x, y});
            }
            while (!q.empty()) {
                auto [x, y] = q.front();
                q.pop();
                if (dist_from_i[x][y] >= dist_between[i][j]) continue;
                if (adj_cells[j].count({x, y})) {
                    dist_between[i][j] = min(dist_between[i][j], dist_from_i[x][y]);
                }
                for (int d_dir = 0; d_dir < 4; d_dir++) {
                    int nx = x + dx[d_dir];
                    int ny = y + dy[d_dir];
                    if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '.' && dist_from_i[nx][ny] > dist_from_i[x][y] + 1) {
                        dist_from_i[nx][ny] = dist_from_i[x][y] + 1;
                        q.push({nx, ny});
                    }
                }
            }
        }
    }

    int best_cost = INF;
    int best_i = -1, best_j = -1;
    int best_option = 0;

    for (int i = 0; i < K; i++) {
        if (d_val[i] == INF) continue;
        for (int j = i+1; j < K; j++) {
            if (d_val[j] == INF) continue;
            int option1 = d_val[i] + d_val[j];
            int option2 = dist_between[i][j] + min(d_val[i], d_val[j]);
            int cost = min(option1, option2);
            if (cost < best_cost) {
                best_cost = cost;
                best_i = i;
                best_j = j;
                best_option = (option1 <= option2) ? 1 : 2;
            }
        }
    }

    vector<vector<bool>> pipe(n, vector<bool>(m, false));
    if (best_option == 1) {
        int i = best_i, j = best_j;
        pair<int, int> cell_i = {-1, -1};
        pair<int, int> cell_j = {-1, -1};
        for (auto [x, y] : adj_cells[i]) {
            if (distOcean[x][y] == d_val[i]) {
                cell_i = {x, y};
                break;
            }
        }
        for (auto [x, y] : adj_cells[j]) {
            if (distOcean[x][y] == d_val[j]) {
                cell_j = {x, y};
                break;
            }
        }

        int x = cell_i.first, y = cell_i.second;
        while (true) {
            pipe[x][y] = true;
            if (x == 0 || x == n-1 || y == 0 || y == m-1) break;
            int nx = -1, ny = -1;
            for (int d = 0; d < 4; d++) {
                int cx = x + dx[d];
                int cy = y + dy[d];
                if (cx >= 0 && cx < n && cy >= 0 && cy < m && grid[cx][cy] == '.' && distOcean[cx][cy] == distOcean[x][y] - 1) {
                    nx = cx;
                    ny = cy;
                    break;
                }
            }
            if (nx == -1) break;
            x = nx;
            y = ny;
        }

        x = cell_j.first, y = cell_j.second;
        while (true) {
            pipe[x][y] = true;
            if (x == 0 || x == n-1 || y == 0 || y == m-1) break;
            int nx = -1, ny = -1;
            for (int d = 0; d < 4; d++) {
                int cx = x + dx[d];
                int cy = y + dy[d];
                if (cx >= 0 && cx < n && cy >= 0 && cy < m && grid[cx][cy] == '.' && distOcean[cx][cy] == distOcean[x][y] - 1) {
                    nx = cx;
                    ny = cy;
                    break;
                }
            }
            if (nx == -1) break;
            x = nx;
            y = ny;
        }
    } else {
        int i = best_i, j = best_j;
        pair<int, int> cell_i = {-1, -1};
        pair<int, int> cell_j = {-1, -1};
        int min_dist = INF;
        if (d_val[i] <= d_val[j]) {
            for (auto [x, y] : adj_cells[i]) {
                for (auto [a, b] : adj_cells[j]) {
                    if (dist_between[i][j] == dist_from_i[a][b]) {
                        if (dist_from_i[x][y] + d_val[i] < min_dist) {
                            min_dist = dist_from_i[x][y] + d_val[i];
                            cell_i = {x, y};
                            cell_j = {a, b};
                        }
                    }
                }
            }
        } else {
            for (auto [x, y] : adj_cells[j]) {
                for (auto [a, b] : adj_cells[i]) {
                    if (dist_between[i][j] == dist_from_i[a][b]) {
                        if (dist_from_i[x][y] + d_val[j] < min_dist) {
                            min_dist = dist_from_i[x][y] + d_val[j];
                            cell_i = {a, b};
                            cell_j = {x, y};
                        }
                    }
                }
            }
        }

        int x = cell_i.first, y = cell_i.second;
        while (true) {
            pipe[x][y] = true;
            if (x == 0 || x == n-1 || y == 0 || y == m-1) break;
            int nx = -1, ny = -1;
            for (int d = 0; d < 4; d++) {
                int cx = x + dx[d];
                int cy = y + dy[d];
                if (cx >= 0 && cx < n && cy >= 0 && cy < m && grid[cx][cy] == '.' && distOcean[cx][cy] == distOcean[x][y] - 1) {
                    nx = cx;
                    ny = cy;
                    break;
                }
            }
            if (nx == -1) break;
            x = nx;
            y = ny;
        }

        vector<vector<int>> dist_from_cell_i(n, vector<int>(m, INF));
        queue<pair<int, int>> q;
        dist_from_cell_i[cell_i.first][cell_i.second] = 0;
        q.push(cell_i);
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '.' && dist_from_cell_i[nx][ny] > dist_from_cell_i[x][y] + 1) {
                    dist_from_cell_i[nx][ny] = dist_from_cell_i[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
        x = cell_j.first, y = cell_j.second;
        while (true) {
            pipe[x][y] = true;
            if (x == cell_i.first && y == cell_i.second) break;
            int nx = -1, ny = -1;
            for (int d = 0; d < 4; d++) {
                int cx = x + dx[d];
                int cy = y + dy[d];
                if (cx >= 0 && cx < n && cy >= 0 && cy < m && grid[cx][cy] == '.' && dist_from_cell_i[cx][cy] == dist_from_cell_i[x][y] - 1) {
                    nx = cx;
                    ny = cy;
                    break;
                }
            }
            if (nx == -1) break;
            x = nx;
            y = ny;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'X') {
                cout << 'X';
            } else if (pipe[i][j]) {
                cout << '*';
            } else {
                cout << '.';
            }
        }
        cout << '\n';
    }

    return 0;
}
