#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> board(n);
    for (int i = 0; i < n; i++) cin >> board[i];

    struct Piece {
        int r, c;
        char type;
    };

    vector<Piece> pieces;
    int start = -1;
    vector<int> pieceAt(n * m, -1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char ch = board[i][j];
            int id = i * m + j;
            if (ch == '*') {
                start = id;
            } else if (ch == 'K' || ch == 'B' || ch == 'R') {
                pieceAt[id] = (int)pieces.size();
                pieces.push_back({i, j, ch});
            }
        }
    }

    int p = (int)pieces.size();
    if (p == 0) {
        cout << 0 << '\n';
        return 0;
    }

    int fullMask = (1 << p) - 1;
    int cells = n * m;

    vector<vector<unsigned char>> attacked(1 << p, vector<unsigned char>(cells, 0));

    int knightDr[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int knightDc[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

    int rookDr[4] = {-1, 1, 0, 0};
    int rookDc[4] = {0, 0, -1, 1};

    int bishopDr[4] = {-1, -1, 1, 1};
    int bishopDc[4] = {-1, 1, -1, 1};

    auto inside = [&](int r, int c) {
        return r >= 0 && r < n && c >= 0 && c < m;
    };

    for (int mask = 0; mask <= fullMask; mask++) {
        for (int i = 0; i < p; i++) {
            if (mask & (1 << i)) continue;

            int r = pieces[i].r;
            int c = pieces[i].c;
            char type = pieces[i].type;

            if (type == 'K') {
                for (int d = 0; d < 8; d++) {
                    int nr = r + knightDr[d];
                    int nc = c + knightDc[d];
                    if (inside(nr, nc)) {
                        attacked[mask][nr * m + nc] = 1;
                    }
                }
            } else {
                vector<pair<int, int>> dirs;

                if (type == 'R') {
                    for (int d = 0; d < 4; d++) dirs.push_back({rookDr[d], rookDc[d]});
                } else {
                    for (int d = 0; d < 4; d++) dirs.push_back({bishopDr[d], bishopDc[d]});
                }

                for (auto [dr, dc] : dirs) {
                    int nr = r + dr;
                    int nc = c + dc;

                    while (inside(nr, nc)) {
                        int cell = nr * m + nc;
                        int blockingPiece = pieceAt[cell];

                        if (blockingPiece != -1 && !(mask & (1 << blockingPiece))) {
                            break;
                        }

                        attacked[mask][cell] = 1;

                        nr += dr;
                        nc += dc;
                    }
                }
            }
        }
    }

    int totalStates = (1 << p) * cells;
    vector<int> dist(totalStates, -1);

    auto stateId = [&](int mask, int pos) {
        return mask * cells + pos;
    };

    queue<pair<int, int>> q;
    dist[stateId(0, start)] = 0;
    q.push({0, start});

    int kingDr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int kingDc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while (!q.empty()) {
        auto [mask, pos] = q.front();
        q.pop();

        int curDist = dist[stateId(mask, pos)];

        if (mask == fullMask) {
            cout << curDist << '\n';
            return 0;
        }

        int r = pos / m;
        int c = pos % m;

        for (int d = 0; d < 8; d++) {
            int nr = r + kingDr[d];
            int nc = c + kingDc[d];

            if (!inside(nr, nc)) continue;

            int npos = nr * m + nc;
            int nmask = mask;

            int idx = pieceAt[npos];
            if (idx != -1 && !(mask & (1 << idx))) {
                nmask |= (1 << idx);
            }

            if (attacked[nmask][npos]) continue;

            int id = stateId(nmask, npos);
            if (dist[id] == -1) {
                dist[id] = curDist + 1;
                q.push({nmask, npos});
            }
        }
    }

    cout << -1 << '\n';
    return 0;
}
