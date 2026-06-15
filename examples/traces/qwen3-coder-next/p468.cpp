#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cctype>
using namespace std;

const int dr[8] = {2, 1, -1, -2, -2, -1, 1, 2};
const int dc[8] = {1, 2, 2, 1, -1, -2, -2, -1};

bool warnsdorff(int N, int start_r, int start_c, vector<vector<int>>& board) {
    board.assign(N, vector<int>(N, 0));
    if (N == 1) {
        board[0][0] = 1;
        return true;
    }
    board[start_r][start_c] = 1;
    int r = start_r, c = start_c;
    int total = N * N;

    for (int step = 2; step <= total; step++) {
        vector<tuple<int, int, int>> candidates; // (exit_count, r, c)

        for (int i = 0; i < 8; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && board[nr][nc] == 0) {
                int cnt = 0;
                for (int j = 0; j < 8; j++) {
                    int nnr = nr + dr[j];
                    int nnc = nc + dc[j];
                    if (nnr >= 0 && nnr < N && nnc >= 0 && nnc < N && board[nnr][nnc] == 0) {
                        cnt++;
                    }
                }
                candidates.push_back(make_tuple(cnt, nr, nc));
            }
        }

        if (candidates.empty()) {
            return false;
        }

        // Sort by: min exit count, then min row, then min col
        sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b) {
            if (get<0>(a) != get<0>(b))
                return get<0>(a) < get<0>(b);
            if (get<1>(a) != get<1>(b))
                return get<1>(a) < get<1>(b);
            return get<2>(a) < get<2>(b);
        });

        r = get<1>(candidates[0]);
        c = get<2>(candidates[0]);
        board[r][c] = step;
    }
    return true;
}

int main() {
    int N;
    cin >> N;

    if (N == 1) {
        cout << "There is solution:\n1\n";
        return 0;
    }

    if (N == 2 || N == 3 || N == 4) {
        cout << "No solution.\n";
        return 0;
    }

    vector<vector<int>> board;
    vector<pair<int, int>> starts = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}};
    bool found = false;

    for (auto& start : starts) {
        if (start.first < N && start.second < N) {
            if (warnsdorff(N, start.first, start.second, board)) {
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "No solution.\n";
    } else {
        cout << "There is solution:\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (j > 0) cout << " ";
                cout << board[i][j];
            }
            cout << "\n";
        }
    }

    return 0;
}
