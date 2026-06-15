#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

int n, m, K;
int grid[15][15];
int best_sum;
vector<pair<int, int>> best_region;
bool selected[15][15];

int dr[] = {0, 0, 1, -1};
int dc[] = {1, -1, 0, 0};

void dfs(int depth, int sum, vector<pair<int, int>>& region, int last_idx) {
    if (depth == K) {
        if (sum > best_sum) {
            best_sum = sum;
            best_region = region;
        }
        return;
    }

    for (int idx = last_idx + 1; idx < n * m; idx++) {
        int r = idx / m;
        int c = idx % m;
        if (selected[r][c]) 
            continue;

        bool adj = false;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && selected[nr][nc]) {
                adj = true;
                break;
            }
        }
        if (!adj) 
            continue;

        selected[r][c] = true;
        region.push_back({r, c});
        dfs(depth + 1, sum + grid[r][c], region, idx);
        region.pop_back();
        selected[r][c] = false;
    }
}

int main() {
    cin >> n >> m >> K;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    if (K == 0) {
        cout << "Oil : 0" << endl;
        return 0;
    }

    best_sum = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            memset(selected, 0, sizeof(selected));
            vector<pair<int, int>> region;
            selected[i][j] = true;
            region.push_back({i, j});
            dfs(1, grid[i][j], region, i * m + j);
        }
    }

    cout << "Oil : " << best_sum << endl;
    for (auto& p : best_region) {
        cout << p.first + 1 << " " << p.second + 1 << endl;
    }

    return 0;
}
