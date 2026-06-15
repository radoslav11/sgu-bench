#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> T(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> T[i][j];
        }
    }

    vector<vector<int>> rem(n, vector<int>(m, 0));
    vector<vector<bool>> chosen(n, vector<bool>(m, false));
    queue<pair<int, int>> q;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int count = 0;
            for (int d = 0; d < 4; ++d) {
                int ni = i + dr[d];
                int nj = j + dc[d];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                    count++;
                }
            }
            rem[i][j] = count;
            if (rem[i][j] == T[i][j] - 1) {
                q.push({i, j});
            }
        }

    }

    vector<pair<int, int>> seq;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (chosen[r][c]) continue;

        if (rem[r][c] != T[r][c] - 1) {
            cout << "No solution\n";
            return 0;
        }

        chosen[r][c] = true;
        seq.push_back({r + 1, c + 1});

        for (int d = 0; d < 4; ++d) {
            int ni = r + dr[d];
            int nj = c + dc[d];
            if (ni >= 0 && ni < n && nj >= 0 && nj < m && !chosen[ni][nj]) {
                rem[ni][nj]--;
                if (rem[ni][nj] == T[ni][nj] - 1) {
                    q.push({ni, nj});
                }
            }
        }
    }

    if (seq.size() != (size_t)(n * m)) {
        cout << "No solution\n";
    } else {
        for (auto p : seq) {
            cout << p.first << " " << p.second << "\n";
        }
    }

    return 0;
}
