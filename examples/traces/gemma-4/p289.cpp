#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Problem: 289. Challenging Tic-Tac-Toe
 * The goal is to determine if a given Tic-Tac-Toe position is legal and, if it is,
 * find the winner assuming perfect play, or if it's a draw.
 * 
 * Rules for a legal position:
 * 1. countX == count0 or countX == count0 + 1.
 * 2. If X wins, then countX must be count0 + 1 and 0 cannot have a winning configuration.
 * 3. If 0 wins, then countX must be count0 and X cannot have a winning configuration.
 * 4. X and 0 cannot both have winning lines.
 * 
 * Perfect play is determined using the minimax algorithm with memoization.
 */

// Check if X has a winning configuration
bool X_wins(const string& b) {
    if (b[0] == 'X' && b[1] == 'X' && b[2] == 'X') return true;
    if (b[3] == 'X' && b[4] == 'X' && b[5] == 'X') return true;
    if (b[6] == 'X' && b[7] == 'X' && b[8] == 'X') return true;
    if (b[0] == 'X' && b[3] == 'X' && b[6] == 'X') return true;
    if (b[1] == 'X' && b[4] == 'X' && b[7] == 'X') return true;
    if (b[2] == 'X' && b[5] == 'X' && b[8] == 'X') return true;
    if (b[0] == 'X' && b[4] == 'X' && b[8] == 'X') return true;
    if (b[2] == 'X' && b[4] == 'X' && b[6] == 'X') return true;
    return false;
}

// Check if 0 has a winning configuration
bool O_wins(const string& b) {
    if (b[0] == '0' && b[1] == '0' && b[2] == '0') return true;
    if (b[3] == '0' && b[4] == '0' && b[5] == '0') return true;
    if (b[6] == '0' && b[7] == '0' && b[8] == '0') return true;
    if (b[0] == '0' && b[3] == '0' && b[6] == '0') return true;
    if (b[1] == '0' && b[4] == '0' && b[7] == '0') return true;
    if (b[2] == '0' && b[5] == '0' && b[8] == '0') return true;
    if (b[0] == '0' && b[4] == '0' && b[8] == '0') return true;
    if (b[2] == '0' && b[4] == '0' && b[6] == '0') return true;
    return false;
}

// Check if the board is full
bool is_full(const string& b) {
    for (char c : b) if (c == '.') return false;
    return true;
}

map<string, int> memo;

// Minimax algorithm to find the perfect play outcome
// 1: X wins, 0: Draw, -1: 0 wins
int minimax(string b, bool is_X_turn) {
    if (X_wins(b)) return 1;
    if (O_wins(b)) return -1;
    if (is_full(b)) return 0;

    if (memo.count(b)) return memo[b];

    if (is_X_turn) {
        int res = -2;
        for (int i = 0; i < 9; ++i) {
            if (b[i] == '.') {
                b[i] = 'X';
                res = max(res, minimax(b, false));
                b[i] = '.';
                if (res == 1) break; // Optimization: X can't do better than winning
            }
        }
        return memo[b] = res;
    } else {
        int res = 2;
        for (int i = 0; i < 9; ++i) {
            if (b[i] == '.') {
                b[i] = '0';
                res = min(res, minimax(b, true));
                b[i] = '.';
                if (res == -1) break; // Optimization: 0 can't do better than winning
            }
        }
        return memo[b] = res;
    }
}

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string line;
    while (cin >> line && line != "Qc") {
        string board = line;
        for (int i = 0; i < 2; ++i) {
            string row;
            cin >> row;
            board += row;
        }

        int countX = 0, count0 = 0;
        for (char c : board) {
            if (c == 'X') countX++;
            else if (c == '0') count0++;
        }

        bool x_win = X_wins(board);
        bool o_win = O_wins(board);

        // Validate if the position is legal
        if (countX < count0 || countX > count0 + 1 ||
            (x_win && o_win) ||
            (x_win && countX != count0 + 1) ||
            (o_win && countX != count0)) {
            cout << "Illegal position." << endl;
        } else if (x_win) {
            cout << "X wins." << endl;
        } else if (o_win) {
            cout << "0 wins." << endl;
        } else {
            // If the game hasn't ended, determine the result with perfect play
            // If countX == count0, it is currently X's turn.
            int res = minimax(board, countX == count0);
            if (res == 1) cout << "X wins." << endl;
            else if (res == -1) cout << "0 wins." << endl;
            else cout << "Game is a draw." << endl;
        }
    }

    return 0;
}

