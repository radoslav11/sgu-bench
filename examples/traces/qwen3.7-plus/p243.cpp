#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int N;
int K;

struct Piece {
    vector<vector<pair<int, int>>> orients;
};

Piece pieces[26];

struct PieceInfo {
    int id;
    int size;
    int num_orients;
};

vector<PieceInfo> piece_order;

vector<pair<int, int>> normalize(const vector<pair<int, int>>& cells) {
    int min_r = 100, min_c = 100;
    for (auto& p : cells) {
        min_r = min(min_r, p.first);
        min_c = min(min_c, p.second);
    }
    vector<pair<int, int>> res;
    for (auto& p : cells) {
        res.push_back({p.first - min_r, p.second - min_c});
    }
    sort(res.begin(), res.end());
    return res;
}

int get_max_r(const vector<pair<int, int>>& cells) {
    int max_r = -1;
    for (auto& p : cells) max_r = max(max_r, p.first);
    return max_r;
}

vector<pair<int, int>> rotate90(const vector<pair<int, int>>& cells) {
    int max_r = get_max_r(cells);
    vector<pair<int, int>> res;
    for (auto& p : cells) {
        res.push_back({p.second, max_r - p.first});
    }
    return normalize(res);
}

vector<vector<pair<int, int>>> get_orientations(const vector<pair<int, int>>& initial_cells) {
    vector<vector<pair<int, int>>> orients;
    auto current = normalize(initial_cells);
    orients.push_back(current);
    for (int i = 0; i < 3; ++i) {
        current = rotate90(current);
        bool found = false;
        for (auto& o : orients) {
            if (o == current) {
                found = true;
                break;
            }
        }
        if (!found) {
            orients.push_back(current);
        }
    }
    return orients;
}

char result_board[5][5];

bool solve_dfs(int step, uint32_t mask) {
    if (step == K) {
        return true;
    }
    int piece_idx = piece_order[step].id;
    for (const auto& orient : pieces[piece_idx].orients) {
        int max_r = get_max_r(orient);
        int max_c = -1;
        for (auto& p : orient) max_c = max(max_c, p.second);
        
        for (int r_shift = 0; r_shift <= N - 1 - max_r; ++r_shift) {
            for (int c_shift = 0; c_shift <= N - 1 - max_c; ++c_shift) {
                bool ok = true;
                uint32_t piece_mask = 0;
                for (auto& p : orient) {
                    int r = p.first + r_shift;
                    int c = p.second + c_shift;
                    int bit = r * N + c;
                    if ((mask >> bit) & 1) {
                        ok = false;
                        break;
                    }
                    piece_mask |= (1u << bit);
                }
                if (ok) {
                    char piece_char = 'A' + piece_idx;
                    for (auto& p : orient) {
                        result_board[p.first + r_shift][p.second + c_shift] = piece_char;
                    }
                    
                    if (solve_dfs(step + 1, mask | piece_mask)) {
                        return true;
                    }
                    
                    for (auto& p : orient) {
                        result_board[p.first + r_shift][p.second + c_shift] = '.';
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (!(cin >> N)) return 0;
    
    vector<string> grid(20);
    char max_char = 'A' - 1;
    for (int i = 0; i < 20; ++i) {
        cin >> grid[i];
        for (char c : grid[i]) {
            if (c >= 'A' && c <= 'Z') {
                if (c > max_char) {
                    max_char = c;
                }
            }
        }
    }
    
    K = max_char - 'A' + 1;
    
    vector<pair<int, int>> current_cells;
    for (char c = 'A'; c <= max_char; ++c) {
        current_cells.clear();
        for (int r = 0; r < 20; ++r) {
            for (int col = 0; col < 20; ++col) {
                if (grid[r][col] == c) {
                    current_cells.push_back({r, col});
                }
            }
        }
        pieces[c - 'A'].orients = get_orientations(current_cells);
        piece_order.push_back({(int)(c - 'A'), (int)current_cells.size(), (int)pieces[c - 'A'].orients.size()});
    }
    
    sort(piece_order.begin(), piece_order.end(), [](const PieceInfo& a, const PieceInfo& b) {
        if (a.size != b.size) return a.size > b.size;
        return a.num_orients < b.num_orients;
    });
    
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            result_board[r][c] = '.';
        }
    }
    
    solve_dfs(0, 0);
    
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            cout << result_board[r][c];
        }
        cout << "\n";
    }
    
    return 0;
}
