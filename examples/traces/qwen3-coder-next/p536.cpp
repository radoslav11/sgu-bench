#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

const int MAX_K = 15;
const int MAX_MASK = 1 << MAX_K;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    // Find white king and black pieces
    int start_r = -1, start_c = -1;
    vector<tuple<int, int, char>> blacks;
    vector<vector<int>> init_black(n, vector<int>(m, -1)); // -1: no black piece

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '*') {
                start_r = i;
                start_c = j;
                grid[i][j] = '.'; // remove king, treat as empty
            } else if (grid[i][j] == 'K' || grid[i][j] == 'B' || grid[i][j] == 'R') {
                init_black[i][j] = blacks.size();
                blacks.push_back({i, j, grid[i][j]});
            }
        }
    }

    int k = blacks.size();
    if (k == 0) {
        cout << 0 << endl;
        return 0;
    }

    // Precompute attack_list and blocker_mask
    vector<vector<vector<int>>> attack_list(n, vector<vector<int>>(m));
    // blocker_mask[i][r][c] = bitmask of black pieces that block black piece i from attacking (r, c)
    vector<vector<vector<int>>> blocker_mask(k, vector<vector<int>>(n, vector<int>(m, 0)));

    // Knight moves
    vector<pair<int, int>> knight_d = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    // Rook and bishop directions
    vector<pair<int, int>> rook_d = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    vector<pair<int, int>> bishop_d = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};

    for (int i = 0; i < k; i++) {
        auto [r0, c0, type] = blacks[i];
        if (type == 'K') {
            for (auto &d : knight_d) {
                int r = r0 + d.first;
                int c = c0 + d.second;
                if (r >= 0 && r < n && c >= 0 && c < m) {
                    attack_list[r][c].push_back(i);
                    // blocker_mask[i][r][c] remains 0 for knight (no blocking)
                }
            }
        } else {
            vector<pair<int, int>> dirs = (type == 'R') ? rook_d : bishop_d;
            for (auto &d : dirs) {
                int r = r0 + d.first;
                int c = c0 + d.second;
                int mask = 0;
                while (r >= 0 && r < n && c >= 0 && c < m) {
                    attack_list[r][c].push_back(i);
                    blocker_mask[i][r][c] = mask;
                    if (init_black[r][c] != -1 && init_black[r][c] != i) {
                        mask |= (1 << init_black[r][c]);
                    }
                    r += d.first;
                    c += d.second;
                }
            }
        }
    }

    // BFS: state (r, c, mask)
    // dist[r][c][mask] = number of moves
    int total_masks = 1 << k;
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(m, vector<int>(total_masks, -1)));
    queue<tuple<int, int, int>> q;

    int start_mask = 0;
    dist[start_r][start_c][start_mask] = 0;
    q.push({start_r, start_c, start_mask});

    // King 8 directions
    vector<int> dr8 = {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> dc8 = {-1, 0, 1, -1, 1, -1, 0, 1};

    auto is_safe = [&](int r, int c, int mask) -> bool {
        for (int i : attack_list[r][c]) {
            if (mask & (1 << i)) {
                continue;
            }
            // Check if there is a non-captured blocker for black piece i attacking (r, c)
            if (blocker_mask[i][r][c] & ~mask) {
                // There is a non-captured piece blocking the attack
                continue;
            }
            return false; // attacked by piece i
        }
        return true;
    };

    while (!q.empty()) {
        auto [r, c, mask] = q.front();
        q.pop();
        int d = dist[r][c][mask];

        if (mask == total_masks - 1) {
            cout << d << endl;
            return 0;
        }

        for (int dir = 0; dir < 8; dir++) {
            int nr = r + dr8[dir];
            int nc = c + dc8[dir];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
                continue;
            }

            // Check safety of (nr, nc) in the current mask state
            if (!is_safe(nr, nc, mask)) {
                continue;
            }

            int new_mask = mask;
            if (init_black[nr][nc] != -1) {
                int idx = init_black[nr][nc];
                if (!(mask & (1 << idx))) {
                    new_mask |= (1 << idx);
                }
            }

            if (dist[nr][nc][new_mask] == -1) {
                dist[nr][nc][new_mask] = d + 1;
                q.push({nr, nc, new_mask});
            }
        }
    }

    cout << -1 << endl;
    return 0;
}
