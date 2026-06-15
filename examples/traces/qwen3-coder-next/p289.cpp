#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool checkWin(vector<string>& b, char c) {
    for (int i = 0; i < 3; i++) {
        if (b[i][0] == c && b[i][1] == c && b[i][2] == c)
            return true;
    }
    for (int j = 0; j < 3; j++) {
        if (b[0][j] == c && b[1][j] == c && b[2][j] == c)
            return true;
    }
    if (b[0][0] == c && b[1][1] == c && b[2][2] == c)
        return true;
    if (b[0][2] == c && b[1][1] == c && b[2][0] == c)
        return true;
    return false;
}

int solve_game(vector<string> board, char player) {
    vector<pair<int, int>> moves;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '.') {
                moves.push_back(make_pair(i, j));
            }
        }
    }

    int best = -2;
    for (auto& m : moves) {
        int i = m.first;
        int j = m.second;
        board[i][j] = player;

        if (checkWin(board, player)) {
            board[i][j] = '.';
            return 1;
        }

        bool full = true;
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if (board[x][y] == '.') {
                    full = false;
                    break;
                }
            }
            if (!full) break;
        }
        if (full) {
            board[i][j] = '.';
            if (0 > best) best = 0;
            continue;
        }

        char opponent = (player == 'X') ? '0' : 'X';
        int val = solve_game(board, opponent);
        int score = -val;
        board[i][j] = '.';

        if (score > best) best = score;
        if (best == 1) break;
    }

    if (best == -2) {
        best = 0;
    }
    return best;
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (line == "Qc") {
            break;
        }

        vector<string> board(3);
        board[0] = line;
        getline(cin, board[1]);
        getline(cin, board[2]);
        getline(cin, line);

        int countX = 0, count0 = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 'X') 
                    countX++;
                else if (board[i][j] == '0') 
                    count0++;
            }
        }

        bool winX = checkWin(board, 'X');
        bool win0 = checkWin(board, '0');

        bool illegal = false;
        if (countX != count0 && countX != count0 + 1) {
            illegal = true;
        }
        if (winX && win0) {
            illegal = true;
        }
        if (winX && countX != count0 + 1) {
            illegal = true;
        }
        if (win0 && countX != count0) {
            illegal = true;
        }

        if (illegal) {
            cout << "Illegal position." << endl;
        } else if (winX) {
            cout << "X wins." << endl;
        } else if (win0) {
            cout << "0 wins." << endl;
        } else if (countX + count0 == 9) {
            cout << "Game is a draw." << endl;
        } else {
            char turn = (countX == count0) ? 'X' : '0';
            int res = solve_game(board, turn);
            if (res == 1) {
                if (turn == 'X') {
                    cout << "X wins." << endl;
                } else {
                    cout << "0 wins." << endl;
                }
            } else if (res == -1) {
                if (turn == 'X') {
                    cout << "0 wins." << endl;
                } else {
                    cout << "X wins." << endl;
                }
            } else {
                cout << "Game is a draw." << endl;
            }
        }
    }
    return 0;
}
