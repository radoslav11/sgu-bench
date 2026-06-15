#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    int M;
    cin >> M;

    vector<string> board(N);
    int qr = -1, qc = -1;

    for (int i = 0; i < N; i++) {
        cin >> board[i];
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'Q') {
                qr = i;
                qc = j;
            }
        }
    }

    vector<vector<char>> cur(N, vector<char>(N, 0));
    cur[qr][qc] = 1;

    auto inside = [&](int r, int c) {
        return r >= 0 && r < N && c >= 0 && c < N;
    };

    for (int move = 0; move < M; move++) {
        vector<vector<char>> nxt(N, vector<char>(N, 0));

        auto scan = [&](int sr, int sc, int dr, int dc) {
            int r = sr, c = sc;
            bool seen = false;

            while (inside(r, c)) {
                char cell = board[r][c];

                if (cell != 'W' && seen) {
                    nxt[r][c] = 1;
                }

                if (cell == 'W' || cell == 'B') {
                    seen = (cell == 'B' && cur[r][c]);
                } else {
                    if (cur[r][c]) seen = true;
                }

                r += dr;
                c += dc;
            }
        };

        // Horizontal
        for (int r = 0; r < N; r++) {
            scan(r, 0, 0, 1);
            scan(r, N - 1, 0, -1);
        }

        // Vertical
        for (int c = 0; c < N; c++) {
            scan(0, c, 1, 0);
            scan(N - 1, c, -1, 0);
        }

        // Main diagonals
        for (int c = 0; c < N; c++) {
            scan(0, c, 1, 1);
            scan(N - 1, c, -1, -1);
        }
        for (int r = 1; r < N; r++) {
            scan(r, 0, 1, 1);
            scan(r - 1, N - 1, -1, -1);
        }

        // Anti-diagonals
        for (int c = 0; c < N; c++) {
            scan(0, c, 1, -1);
            scan(N - 1, c, -1, 1);
        }
        for (int r = 1; r < N; r++) {
            scan(r, N - 1, 1, -1);
            scan(r - 1, 0, -1, 1);
        }

        cur.swap(nxt);
    }

    int answer = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (cur[i][j]) answer++;
        }
    }

    cout << answer << '\n';

    return 0;
}
