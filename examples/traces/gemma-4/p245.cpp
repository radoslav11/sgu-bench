#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

typedef long long ll;

int N, M;
ll bonuses[6], penalty;
vector<string> board;
int start_r, start_c;

bool is_valid(int r, int c) { return r >= 0 && r < N && c >= 0 && c < M; }
bool is_piece(int r, int c) {
    char ch = board[r][c];
    return ch == 'P' || ch == 'R' || ch == 'K' || ch == 'B' || ch == 'Q' || ch == 'M';
}
int get_p_type(int r, int c) {
    char ch = board[r][c];
    if (ch == 'P') return 0; if (ch == 'R') return 1; if (ch == 'K') return 2;
    if (ch == 'B') return 3; if (ch == 'Q') return 4; if (ch == 'M') return 5;
    return -1;
}

void mark_attacks(vector<vector<bool>>& unsafe) {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < M; ++c) {
            if (is_piece(r, c)) {
                int t = get_p_type(r, c);
                if (t == 0) {
                    int dr[] = {-1, -1}, dc[] = {-1, 1};
                    for (int i = 0; i < 2; ++i) {
                        int nr = r + dr[i], nc = c + dc[i];
                        if (is_valid(nr, nc)) unsafe[nr][nc] = true;
                    }
                } else if (t == 1) {
                    int dr[] = {0, 0, 1, -1}, dc[] = {1, -1, 0, 0};
                    for (int i = 0; i < 4; ++i) {
                        for (int d = 1; ; ++d) {
                            int nr = r + dr[i] * d, nc = c + dc[i] * d;
                            if (!is_valid(nr, nc) || board[nr][nc] == '#') break;
                            unsafe[nr][nc] = true;
                            if (is_piece(nr, nc)) break;
                        }
                    }
                } else if (t == 2) {
                    int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2}, dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
                    for (int i = 0; i < 8; ++i) {
                        int nr = r + dr[i], nc = c + dc[i];
                        if (is_valid(nr, nc)) unsafe[nr][nc] = true;
                    }
                } else if (t == 3) {
                    int dr[] = {1, 1, -1, -1}, dc[] = {1, -1, 1, -1};
                    for (int i = 0; i < 4; ++i) {
                        for (int d = 1; ; ++d) {
                            int nr = r + dr[i] * d, nc = c + dc[i] * d;
                            if (!is_valid(nr, nc) || board[nr][nc] == '#') break;
                            unsafe[nr][nc] = true;
                            if (is_piece(nr, nc)) break;
                        }
                    }
                } else if (t == 4) {
                    int dr[] = {0, 0, 1, -1, 1, 1, -1, -1}, dc[] = {1, -1, 0, 0, 1, -1, 1, -1};
                    for (int i = 0; i < 8; ++i) {
                        for (int d = 1; ; ++d) {
                            int nr = r + dr[i] * d, nc = c + dc[i] * d;
                            if (!is_valid(nr, nc) || board[nr][nc] == '#') break;
                            unsafe[nr][nc] = true;
                            if (is_piece(nr, nc)) break;
                        }
                    }
                } else if (t == 5) {
                    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
                    for (int i = 0; i < 8; ++i) {
                        int nr = r + dr[i], nc = c + dc[i];
                        if (is_valid(nr, nc)) unsafe[nr][nc] = true;
                    }
                }
            }
        }
    }
}

void get_moves(int r, int c, int utype, vector<pair<int, int>>& moves) {
    if (utype == 0) {
        int dr[] = {1, 1}, dc[] = {-1, 1};
        for (int i = 0; i < 2; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (is_valid(nr, nc) && is_piece(nr, nc)) moves.push_back({nr, nc});
        }
        int nr = r + 1, nc = c;
        if (is_valid(nr, nc) && (board[nr][nc] == '.' || board[nr][nc] == '@')) moves.push_back({nr, nc});
    } else if (utype == 1) {
        int dr[] = {0, 0, 1, -1}, dc[] = {1, -1, 0, 0};
        for (int i = 0; i < 4; ++i) {
            for (int d = 1; ; ++d) {
                int nr = r + dr[i] * d, nc = c + dc[i] * d;
                if (!is_valid(nr, nc) || board[nr][nc] == '#') break;
                moves.push_back({nr, nc});
                if (is_piece(nr, nc)) break;
            }
        }
    } else if (utype == 2) {
        int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2}, dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (is_valid(nr, nc) && board[nr][nc] != '#') moves.push_back({nr, nc});
        }
    } else if (utype == 3) {
        int dr[] = {1, 1, -1, -1}, dc[] = {1, -1, 1, -1};
        for (int i = 0; i < 4; ++i) {
            for (int d = 1; ; ++d) {
                int nr = r + dr[i] * d, nc = c + dc[i] * d;
                if (!is_valid(nr, nc) || board[nr][nc] == '#') break;
                moves.push_back({nr, nc});
                if (is_piece(nr, nc)) break;
            }
        }
    } else if (utype == 5) {
        int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (is_valid(nr, nc) && board[nr][nc] != '#') moves.push_back({nr, nc});
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    if (!(cin >> N >> M)) return 0;
    for (int i = 0; i < 6; ++i) cin >> bonuses[i];
    cin >> penalty;
    board.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> board[i];
        for (int j = 0; j < M; ++j) if (board[i][j] == '@') { start_r = i; start_c = j; }
    }
    vector<vector<bool>> unsafe(N, vector<bool>(M, false));
    mark_attacks(unsafe);
    ll max_score = 0;
    int types[] = {0, 1, 2, 3, 5};
    for (int type : types) {
        vector<vector<int>> comp(N, vector<int>(M, -1));
        int comp_count = 0;
        vector<ll> comp_bonus;
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < M; ++c) {
                if (!unsafe[r][c] && board[r][c] != '#' && comp[r][c] == -1) {
                    int cid = comp_count++;
                    comp_bonus.push_back(0);
                    queue<pair<int, int>> q;
                    q.push({r, c});
                    comp[r][c] = cid;
                    while (!q.empty()) {
                        pair<int, int> curr = q.front(); q.pop();
                        if (is_piece(curr.first, curr.second)) comp_bonus[cid] += bonuses[get_p_type(curr.first, curr.second)];
                        else {
                            vector<pair<int, int>> moves;
                            get_moves(curr.first, curr.second, type, moves);
                            for (auto& m : moves) {
                                if (is_valid(m.first, m.second) && !unsafe[m.first][m.second] && board[m.first][m.second] != '#' && comp[m.first][m.second] == -1) {
                                    comp[m.first][m.second] = cid;
                                    q.push(m);
                                }
                            }
                        }
                    }
                }
            }
        }
        int start_cid = comp[start_r][start_c];
        if (start_cid != -1) {
            max_score = max(max_score, comp_bonus[start_cid]);
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < M; ++c) {
                    if (comp[r][c] == start_cid) {
                        vector<pair<int, int>> moves;
                        get_moves(r, c, type, moves);
                        for (auto& m : moves) {
                            if (is_valid(m.first, m.second) && is_piece(m.first, m.second) && unsafe[m.first][m.second]) {
                                max_score = max(max_score, comp_bonus[start_cid] + bonuses[get_p_type(m.first, m.second)] - penalty);
                            }
                        }
                    }
                }
            }
        } else {
            vector<pair<int, int>> moves;
            get_moves(start_r, start_c, type, moves);
            for (auto& m : moves) {
                if (is_valid(m.first, m.second) && is_piece(m.first, m.second)) {
                    max_score = max(max_score, bonuses[get_p_type(m.first, m.second)] - (unsafe[m.first][m.second] ? penalty : 0));
                }
            }
        }
    }
    cout << max_score << endl;
    return 0;
}

