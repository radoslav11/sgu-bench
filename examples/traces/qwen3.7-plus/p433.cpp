#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int N, M, K;
vector<int> valid_lengths;
vector<int> grid;
vector<vector<int>> brick_id;
int current_brick_id = 0;

bool dfs() {
    int first_r = -1, first_c = -1;
    for (int i = 0; i < N; ++i) {
        if (grid[i] != (1 << M) - 1) {
            first_r = i;
            break;
        }
    }
    if (first_r == -1) {
        return true;
    }
    
    int mask = ~grid[first_r] & ((1 << M) - 1);
    first_c = __builtin_ctz(mask);
    
    for (int L : valid_lengths) {
        // Try horizontal placement
        if (first_c + L <= M) {
            int req_mask = ((1 << L) - 1) << first_c;
            if ((grid[first_r] & req_mask) == 0) {
                grid[first_r] |= req_mask;
                current_brick_id++;
                for (int i = 0; i < L; ++i) {
                    brick_id[first_r][first_c + i] = current_brick_id;
                }
                if (dfs()) return true;
                current_brick_id--;
                grid[first_r] &= ~req_mask;
            }
        }
        // Try vertical placement
        if (first_r + L <= N) {
            bool ok = true;
            for (int i = 0; i < L; ++i) {
                if (grid[first_r + i] & (1 << first_c)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                for (int i = 0; i < L; ++i) {
                    grid[first_r + i] |= (1 << first_c);
                    brick_id[first_r + i][first_c] = current_brick_id;
                }
                current_brick_id++;
                if (dfs()) return true;
                current_brick_id--;
                for (int i = 0; i < L; ++i) {
                    grid[first_r + i] &= ~(1 << first_c);
                }
            }
        }
    }
    return false;
}

vector<char> brick_color;

bool color_bricks(int idx) {
    if (idx > current_brick_id) return true;
    for (char c = 'a'; c <= 'd'; ++c) {
        bool ok = true;
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < M; ++x) {
                if (brick_id[y][x] == idx) {
                    int dy[] = {-1, 1, 0, 0};
                    int dx[] = {0, 0, -1, 1};
                    for (int d = 0; d < 4; ++d) {
                        int ny = y + dy[d], nx = x + dx[d];
                        if (ny >= 0 && ny < N && nx >= 0 && nx < M) {
                            int n_id = brick_id[ny][nx];
                            if (n_id != 0 && n_id != idx && brick_color[n_id] == c) {
                                ok = false;
                            }
                        }
                    }
                }
            }
        }
        if (ok) {
            brick_color[idx] = c;
            if (color_bricks(idx + 1)) return true;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> N >> M >> K)) return 0;
    
    vector<int> lengths;
    for (int i = 0; i < K; ++i) {
        int L;
        cin >> L;
        if (L <= max(N, M)) {
            lengths.push_back(L);
        }
    }
    
    sort(lengths.rbegin(), lengths.rend());
    lengths.erase(unique(lengths.begin(), lengths.end()), lengths.end());
    valid_lengths = lengths;
    
    grid.assign(N, 0);
    brick_id.assign(N, vector<int>(M, 0));
    
    if (dfs()) {
        cout << "YES\n";
        brick_color.assign(current_brick_id + 1, 0);
        color_bricks(1);
        
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < M; ++x) {
                cout << brick_color[brick_id[y][x]];
            }
            cout << "\n";
        }
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
