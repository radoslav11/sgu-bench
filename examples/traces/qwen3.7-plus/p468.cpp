#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Move {
    int r, c;
    int degree;
};

mt19937 rng(1337);

bool try_solve(int N, vector<vector<int>>& board) {
    board.assign(N, vector<int>(N, 0));
    int r = rng() % N;
    int c = rng() % N;
    board[r][c] = 1;
    
    int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    
    Move moves[8];
    for (int step = 2; step <= N * N; ++step) {
        int move_count = 0;
        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && board[nr][nc] == 0) {
                int degree = 0;
                for (int j = 0; j < 8; ++j) {
                    int nnr = nr + dr[j];
                    int nnc = nc + dc[j];
                    if (nnr >= 0 && nnr < N && nnc >= 0 && nnc < N && board[nnr][nnc] == 0) {
                        degree++;
                    }
                }
                moves[move_count++] = {nr, nc, degree};
            }
        }
        
        if (move_count == 0) {
            return false;
        }
        
        sort(moves, moves + move_count, [](const Move& a, const Move& b) {
            return a.degree < b.degree;
        });
        
        int min_degree = moves[0].degree;
        int count = 1;
        while (count < move_count && moves[count].degree == min_degree) {
            count++;
        }
        
        for (int i = 0; i < count; ++i) {
            int j = i + rng() % (count - i);
            swap(moves[i], moves[j]);
        }
        
        r = moves[0].r;
        c = moves[0].c;
        board[r][c] = step;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    if (!(cin >> N)) return 0;
    
    if (N == 1) {
        cout << "There is solution:\n1\n";
        return 0;
    }
    if (N == 2 || N == 3 || N == 4) {
        cout << "No solution.\n";
        return 0;
    }
    
    vector<vector<int>> board;
    while (!try_solve(N, board)) {
        // Retry with different random choices if Warnsdorff's heuristic gets stuck
    }
    
    cout << "There is solution:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << board[i][j] << (j == N - 1 ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
