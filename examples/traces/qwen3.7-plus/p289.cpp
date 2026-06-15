#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

bool check_win(const string& board, char c) {
    for (int i = 0; i < 3; i++) {
        if (board[i*3] == c && board[i*3+1] == c && board[i*3+2] == c) return true;
        if (board[i] == c && board[i+3] == c && board[i+6] == c) return true;
    }
    if (board[0] == c && board[4] == c && board[8] == c) return true;
    if (board[2] == c && board[4] == c && board[6] == c) return true;
    return false;
}

bool is_legal(const string& board) {
    int count_x = 0, count_0 = 0;
    for (char c : board) {
        if (c == 'X') count_x++;
        else if (c == '0') count_0++;
    }
    if (count_x != count_0 && count_x != count_0 + 1) return false;
    bool win_x = check_win(board, 'X');
    bool win_0 = check_win(board, '0');
    if (win_x && win_0) return false;
    if (win_x && count_x != count_0 + 1) return false;
    if (win_0 && count_x != count_0) return false;
    return true;
}

unordered_map<string, int> memo;

int minimax(string board, bool is_x_turn) {
    if (check_win(board, 'X')) return 1;
    if (check_win(board, '0')) return -1;
    if (board.find('.') == string::npos) return 0;
    
    if (memo.count(board)) return memo[board];

    if (is_x_turn) {
        int max_eval = -2;
        for (int i = 0; i < 9; i++) {
            if (board[i] == '.') {
                board[i] = 'X';
                int eval = minimax(board, false);
                board[i] = '.';
                if (eval > max_eval) max_eval = eval;
                if (max_eval == 1) break; 
            }
        }
        return memo[board] = max_eval;
    } else {
        int min_eval = 2;
        for (int i = 0; i < 9; i++) {
            if (board[i] == '.') {
                board[i] = '0';
                int eval = minimax(board, true);
                board[i] = '.';
                if (eval < min_eval) min_eval = eval;
                if (min_eval == -1) break;
            }
        }
        return memo[board] = min_eval;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string line;
    while (cin >> line) {
        if (line == "Qc") break;
        string board = line;
        cin >> line; board += line;
        cin >> line; board += line;
        
        if (!is_legal(board)) {
            cout << "Illegal position.\n";
        } else {
            if (check_win(board, 'X')) {
                cout << "X wins.\n";
            } else if (check_win(board, '0')) {
                cout << "0 wins.\n";
            } else {
                int cx = 0, co = 0;
                for (char c : board) {
                    if (c == 'X') cx++;
                    else if (c == '0') co++;
                }
                bool is_x_turn = (cx == co);
                int res = minimax(board, is_x_turn);
                if (res == 1) cout << "X wins.\n";
                else if (res == -1) cout << "0 wins.\n";
                else cout << "Game is a draw.\n";
            }
        }
    }
    return 0;
}
