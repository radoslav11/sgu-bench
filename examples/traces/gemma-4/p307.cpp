#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * Problem: 307. Cipher
 * Task: Given an (H-1)x(W-1) fingerprint matrix B, find an HxW binary matrix A such that
 * B[i][j] = A[i][j] + A[i+1][j] + A[i][j+1] + A[i+1][j+1].
 * Constraints: H, W <= 300, Time: 0.25s.
 * 
 * Approach:
 * The relation A[i+1][j+1] = B[i][j] - A[i][j] - A[i+1][j] - A[i][j+1] means that
 * if we know the entire first row and the first column of A, all other elements
 * of A are uniquely determined. We can use backtracking to pick the elements
 * of the first row and then, for each subsequent row, pick the first element
 * and use the recurrence to determine the remaining elements of that row.
 * The pruning will occur as soon as any element is forced to be something other than 0 or 1.
 */

int H, W;
int A[305][305];
int B[305][305];

// Recursively try to find a valid configuration for the matrix A.
// r is the current row we are deciding (1-indexed).
bool solve(int r) {
    if (r > H) {
        return true; // Successfully filled the entire matrix.
    }

    if (r == 1) {
        // For the first row, we have W elements that can be 0 or 1.
        // We iterate through columns to branch.
        // However, to avoid deep recursion, we handle the first row slightly differently.
        // Let's use a recursive function that moves through the columns of the first row.
        return false; // This part should be handled in a specialized way to avoid stack issues.
    }
    return false;
}

// Specialized backtracking to manage row-by-row filling.
// idx_row is the current row (1 to H).
// idx_col is the current column (1 to W).
bool backtrack(int idx_row, int idx_col) {
    // Base case: if we have processed all rows.
    if (idx_row > H) {
        return true;
    }

    // If we are in the first row or the first column, we can try 0 and 1.
    if (idx_row == 1 || idx_col == 1) {
        // Calculate next positions.
        int next_row = idx_row;
        int next_col = idx_col + 1;
        if (next_col > W) {
            next_row = idx_row + 1;
            next_col = 1;
        }

        // Try 0.
        A[idx_row][idx_col] = 0;
        if (backtrack(next_row, next_col)) return true;

        // Try 1.
        A[idx_row][idx_col] = 1;
        if (backtrack(next_row, next_col)) return true;

        return false;
    } else {
        // For any other cell, the value is forced by the fingerprint B.
        // A[idx_row][idx_col] = B[idx_row-1][idx_col-1] - A[idx_row-1][idx_col-1] - A[idx_row-1][idx_col] - A[idx_row][idx_col-1]
        int val = B[idx_row - 1][idx_col - 1] - A[idx_row - 1][idx_col - 1] - A[idx_row - 1][idx_col] - A[idx_row][idx_col - 1];
        if (val == 0 || val == 1) {
            A[idx_row][idx_col] = val;
            // Move to the next cell.
            int next_row = idx_row;
            int next_col = idx_col + 1;
            if (next_col > W) {
                next_row = idx_row + 1;
                next_col = 1;
            }
            return backtrack(next_row, next_col);
        } else {
            return false; // Forced value is not 0 or 1.
        }
    }
}

int main() {
    // Faster I/O.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> H >> W)) return 0;

    for (int i = 1; i < H; ++i) {
        string line;
        cin >> line;
        for (int j = 1; j < W; ++j) {
            B[i][j] = line[j - 1] - '0';
        }
    }

    // Start backtracking from cell (1, 1).
    if (backtrack(1, 1)) {
        for (int i = 1; i <= H; ++i) {
            for (int j = 1; j <= W; ++j) {
                cout << A[i][j];
            }
            cout << "\n";
        }
    } else {
        cout << "CORRUPT" << endl;
    }

    return 0;
}

