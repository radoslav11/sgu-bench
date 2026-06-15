#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<string> grid(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];

    vector<vector<int>> cnt(N, vector<int>(M, 0));
    queue<pair<int, int>> q;

    int occupied = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] == 'X') {
                q.push({i, j});
                occupied++;
            }
        }
    }

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
            if (grid[nx][ny] == 'X') continue;

            cnt[nx][ny]++;

            if (cnt[nx][ny] >= 2) {
                grid[nx][ny] = 'X';
                occupied++;
                q.push({nx, ny});
            }
        }
    }

    cout << occupied << '\n';

    return 0;
}
