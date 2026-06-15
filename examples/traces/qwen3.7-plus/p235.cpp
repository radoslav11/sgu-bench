#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool curr_board[305][305];
bool next_board[305][305];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<string> board(N);
    for (int r = 0; r < N; ++r) {
        cin >> board[r];
    }

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            curr_board[r][c] = false;
            if (board[r][c] == 'Q') {
                curr_board[r][c] = true;
            }
        }
    }

    for (int step = 0; step < M; ++step) {
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                next_board[r][c] = false;
            }
        }

        // Rows
        for (int r = 0; r < N; ++r) {
            bool has = false;
            for (int c = 0; c < N; ++c) {
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
            has = false;
            for (int c = N - 1; c >= 0; --c) {
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
        }

        // Cols
        for (int c = 0; c < N; ++c) {
            bool has = false;
            for (int r = 0; r < N; ++r) {
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
            has = false;
            for (int r = N - 1; r >= 0; --r) {
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
        }

        // Diag 1 (r - c = d)
        for (int d = -(N - 1); d <= N - 1; ++d) {
            int start_r = max(0, d);
            int end_r = min(N - 1, N - 1 + d);
            bool has = false;
            for (int r = start_r; r <= end_r; ++r) {
                int c = r - d;
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
            has = false;
            for (int r = end_r; r >= start_r; --r) {
                int c = r - d;
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
        }

        // Diag 2 (r + c = s)
        for (int s = 0; s <= 2 * N - 2; ++s) {
            int start_r = max(0, s - (N - 1));
            int end_r = min(N - 1, s);
            bool has = false;
            for (int r = start_r; r <= end_r; ++r) {
                int c = s - r;
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
            has = false;
            for (int r = end_r; r >= start_r; --r) {
                int c = s - r;
                if (board[r][c] == 'W') has = false;
                else if (board[r][c] == 'B') {
                    if (has) next_board[r][c] = true;
                    has = curr_board[r][c];
                } else {
                    if (has) next_board[r][c] = true;
                    if (curr_board[r][c]) has = true;
                }
            }
        }

        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                curr_board[r][c] = next_board[r][c];
            }
        }
    }

    int ans = 0;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (curr_board[r][c]) {
                ans++;
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
