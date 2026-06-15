#include<bits/stdc++.h>
using namespace std;

int n, k;
int result = 0;

// Check if we can place a queen at (row, col)
bool isSafe(vector<vector<int>>& board, int row, int col) {
    // Check column
    for (int i = 0; i < n; i++) {
        if (board[i][col] == 1) return false;
    }
    
    // Check row
    for (int j = 0; j < n; j++) {
        if (board[row][j] == 1) return false;
    }
    
    // Check upper left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) return false;
    }
    
    // Check upper right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) return false;
    }
    
    // Check lower left diagonal
    for (int i = row + 1, j = col - 1; i < n && j >= 0; i++, j--) {
        if (board[i][j] == 1) return false;
    }
    
    // Check lower right diagonal
    for (int i = row + 1, j = col + 1; i < n && j < n; i++, j++) {
        if (board[i][j] == 1) return false;
    }
    
    return true;
}

void solve(vector<vector<int>>& board, int queens_placed, int start_pos) {
    // If we've placed k queens, we found a valid configuration
    if (queens_placed == k) {
        result++;
        return;
    }
    
    // If no more positions to check, return
    if (start_pos == n * n) {
        return;
    }
    
    // Pruning: if we can't possibly place k queens even if we fill all remaining squares
    if (queens_placed + (n * n - start_pos) < k) {
        return;
    }
    
    // Try placing a queen at each remaining position
    for (int pos = start_pos; pos < n * n; pos++) {
        int row = pos / n;
        int col = pos % n;
        
        if (isSafe(board, row, col)) {
            board[row][col] = 1;
            solve(board, queens_placed + 1, pos + 1);
            board[row][col] = 0;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> k;
    
    vector<vector<int>> board(n, vector<int>(n, 0));
    
    solve(board, 0, 0);
    
    cout << result << endl;
    
    return 0;
}
