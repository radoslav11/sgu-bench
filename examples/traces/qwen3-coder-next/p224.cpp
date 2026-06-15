#include <iostream>
#include <vector>
using namespace std;

int n, k;
long long result = 0;

// Check if placing a queen at (row, col) is safe
bool isSafe(const vector<int>& queens, int row, int col) {
    for (int r = 0; r < row; ++r) {
        int c = queens[r];
        // Same column
        if (c == col) return false;
        // Same diagonal: |row - r| == |col - c|
        if (abs(row - r) == abs(col - c)) return false;
    }
    return true;
}

// Backtracking: place queens row by row
void backtrack(int row, int placed) {
    // If we've placed k queens, we found a valid configuration
    if (placed == k) {
        result++;
        return;
    }
    
    // If we've gone past the last row, or if remaining rows cannot accommodate remaining queens
    if (row == n) return;
    
    // If even if we place queens in all remaining rows, we can't reach k queens
    if (placed + (n - row) < k) return;
    
    // Try placing a queen in each column of current row
    for (int col = 0; col < n; ++col) {
        if (isSafe(queens, row, col)) {
            queens[row] = col;
            backtrack(row + 1, placed + 1);
            queens[row] = -1; // backtrack
        }
    }
    
    // Also try skipping current row (don't place any queen here)
    backtrack(row + 1, placed);
}

int main() {
    cin >> n >> k;
    
    // Special case: k == 0, only 1 way (empty board)
    if (k == 0) {
        cout << 1 << endl;
        return 0;
    }
    
    vector<int> queens(n, -1); // stores column position for each row
    backtrack(0, 0);
    cout << result << endl;
    
    return 0;
}
