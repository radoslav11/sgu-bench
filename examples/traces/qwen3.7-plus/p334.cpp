#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>

using namespace std;

struct Cell {
    int r, c;
};

int part_arr[9];
int best_part[9];
int best_dc[9], best_dr[9];
bool found = false;

bool is_connected(const vector<int>& subset, const vector<Cell>& C) {
    if (subset.size() <= 1) return true;
    vector<bool> vis(subset.size(), false);
    queue<int> q;
    q.push(0);
    vis[0] = true;
    int count = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t v = 0; v < subset.size(); ++v) {
            if (!vis[v]) {
                if (abs(C[subset[u]].r - C[subset[v]].r) + abs(C[subset[u]].c - C[subset[v]].c) == 1) {
                    vis[v] = true;
                    count++;
                    q.push(v);
                }
            }
        }
    }
    return count == (int)subset.size();
}

bool solve_translations(int i, int mask, int K, const vector<vector<int>>& subsets, const vector<Cell>& C) {
    if (i == K) {
        return mask == 511;
    }
    
    int idx = subsets[i][0];
    int cr = C[idx].r;
    int cc = C[idx].c;
    
    int available = 511 ^ mask;
    while (available) {
        int b = __builtin_ctz(available);
        available &= available - 1;
        
        int tc = b % 3;
        int tr = b / 3;
        int dc = tc - cc;
        int dr = tr - cr;
        
        bool ok = true;
        int new_mask = mask;
        for (int c_idx : subsets[i]) {
            int nr = C[c_idx].r + dr;
            int nc = C[c_idx].c + dc;
            if (nc < 0 || nc >= 3 || nr < 0 || nr >= 3) {
                ok = false; break;
            }
            int bit = nr * 3 + nc;
            if ((new_mask >> bit) & 1) {
                ok = false; break;
            }
            new_mask |= (1 << bit);
        }
        if (ok) {
            if (solve_translations(i + 1, new_mask, K, subsets, C)) {
                best_dc[i] = dc;
                best_dr[i] = dr;
                return true;
            }
        }
    }
    return false;
}

void check_partition(int K, const vector<Cell>& C) {
    vector<vector<int>> subsets(K);
    for (int i = 0; i < 9; ++i) {
        subsets[part_arr[i]].push_back(i);
    }
    
    for (int i = 0; i < K; ++i) {
        if (!is_connected(subsets[i], C)) return;
    }
    
    if (solve_translations(0, 0, K, subsets, C)) {
        for (int i = 0; i < 9; ++i) best_part[i] = part_arr[i];
        found = true;
    }
}

void gen(int idx, int max_part, int num_parts, int K, const vector<Cell>& C) {
    if (found) return;
    if (idx == 9) {
        if (num_parts == K) {
            check_partition(K, C);
        }
        return;
    }
    for (int p = 0; p <= max_part + 1; ++p) {
        if (num_parts + (p == max_part + 1) > K) continue;
        part_arr[idx] = p;
        gen(idx + 1, max(max_part, p), num_parts + (p == max_part + 1), K, C);
    }
}

void output_answer(int K, const vector<Cell>& C, int H, int W, const vector<string>& grid, const vector<vector<int>>& x_idx) {
    cout << K << "\n";
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            if (grid[r][c] == 'X') {
                int idx = x_idx[r][c];
                int p = best_part[idx];
                cout << (char)('A' + p);
            } else {
                cout << '.';
            }
        }
        cout << "\n";
    }
    cout << "\n";
    
    char grid3x3[3][3];
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            grid3x3[r][c] = '.';
        }
    }
    for (int i = 0; i < 9; ++i) {
        int p = best_part[i];
        int nr = C[i].r + best_dr[p];
        int nc = C[i].c + best_dc[p];
        grid3x3[nr][nc] = 'A' + p;
    }
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            cout << grid3x3[r][c];
        }
        cout << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int H, W;
    if (!(cin >> H >> W)) return 0;
    
    vector<string> grid(H);
    vector<Cell> C;
    vector<vector<int>> x_idx(H, vector<int>(W, -1));
    
    for (int r = 0; r < H; ++r) {
        cin >> grid[r];
        for (int c = 0; c < W; ++c) {
            if (grid[r][c] == 'X') {
                x_idx[r][c] = C.size();
                C.push_back({r, c});
            }
        }
    }
    
    for (int K = 1; K <= 9; ++K) {
        part_arr[0] = 0;
        gen(1, 0, 1, K, C);
        if (found) {
            output_answer(K, C, H, W, grid, x_idx);
            return 0;
        }
    }
    
    return 0;
}
