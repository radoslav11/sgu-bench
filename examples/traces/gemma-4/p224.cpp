#include <iostream>

/**
 * Problem: Little Queens
 * Description: Find the number of ways to place k identical queens on an n x n chessboard 
 * such that no two queens attack each other. A queen attacks another if they are in the same
 * row, column, or diagonal.
 * Constraints: n up to 10, k up to n^2.
 * 
 * Approach:
 * Since n is small (n <= 10), we can use backtracking to find all valid placements.
 * Each queen must be in a unique row, so we can iterate row by row and decide 
 * whether to place a queen in the current row and, if so, in which column.
 * 
 * Time complexity: The number of recursive calls is significantly pruned by the attacking conditions.
 * For n=10, the maximum number of solutions is small, and the backtracking explores a limited number of states.
 * Space complexity: O(n) for the recursion stack.
 */

using namespace std;

int N, K;

/**
 * Backtracking function to count valid queen placements.
 * @param row Current row being considered (0 to N-1).
 * @param queens_left Number of queens still to be placed.
 * @param col_mask Bitmask for columns already occupied by a queen.
 * @param diag1_mask Bitmask for main diagonals (r-c) already occupied.
 * @param diag2_mask Bitmask for anti-diagonals (r+c) already occupied.
 * @return Total number of ways to place the remaining queens.
 */
long long solve(int row, int queens_left, int col_mask, int diag1_mask, int diag2_mask) {
    // Base case: All queens have been placed.
    if (queens_left == 0) {
        return 1;
    }
    // Base case: Reached the end of the board without placing all queens.
    if (row == N) {
        return 0;
    }
    // Pruning: If the number of remaining rows is less than the number of queens left to place.
    if (N - row < queens_left) {
        return 0;
    }

    // Option 1: Do not place a queen in the current row.
    long long count = solve(row + 1, queens_left, col_mask, diag1_mask, diag2_mask);

    // Option 2: Place a queen in the current row at column 'c'.
    for (int c = 0; c < N; ++c) {
        // Check if column 'c' and the two diagonals passing through (row, c) are unoccupied.
        // diag1 index: row - c + N (to make it non-negative).
        // diag2 index: row + c.
        int d1 = row - c + N;
        int d2 = row + c;
        if (!(col_mask & (1 << c)) && 
            !(diag1_mask & (1 << d1)) && 
            !(diag2_mask & (1 << d2))) {
            count += solve(row + 1, queens_left - 1, 
                           col_mask | (1 << c), 
                           diag1_mask | (1 << d1), 
                           diag2_mask | (1 << d2));
        }
    }

    return count;
}

int main() {
    // Optimize input/output performance.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> K)) return 0;

    // If K is greater than N, it's impossible to place K queens such that no two are in the same row.
    if (K > N) {
        cout << 0 << endl;
        return 0;
    }
    
    // Special case for K=0: there's exactly one way to place zero queens (an empty board).
    if (K == 0) {
        cout << 1 << endl;
        return 0;
    }

    // Start the backtracking process.
    // Masks are initialized to 0 because no rows/columns/diagonals are occupied yet.
    cout << solve(0, K, 0, 0, 0) << endl;

    return 0;
}

