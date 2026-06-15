#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// Solve A * x = b over GF(2), return solution vector if exists, empty if not
// A is m x n matrix, b is length m vector
vector<int> solveGF2(const vector<vector<int>>& A, const vector<int>& b) {
    int m = A.size();
    int n = A[0].size();
    
    // Augmented matrix [A | b]
    vector<vector<int>> aug(m, vector<int>(n + 1));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = A[i][j];
        }
        aug[i][n] = b[i];
    }
    
    vector<int> x(n, 0);
    vector<int> pivot_col(m, -1);
    int pivot_row = 0;
    
    // Forward elimination
    for (int col = 0; col < n && pivot_row < m; col++) {
        // Find pivot
        int selected = -1;
        for (int row = pivot_row; row < m; row++) {
            if (aug[row][col] == 1) {
                selected = row;
                break;
            }
        }
        if (selected == -1) continue;
        
        // Swap rows
        swap(aug[pivot_row], aug[selected]);
        pivot_col[pivot_row] = col;
        
        // Eliminate
        for (int row = 0; row < m; row++) {
            if (row != pivot_row && aug[row][col] == 1) {
                for (int j = 0; j <= n; j++) {
                    aug[row][j] ^= aug[pivot_row][j];
                }
            }
        }
        pivot_row++;
    }
    
    // Check consistency
    for (int row = pivot_row; row < m; row++) {
        if (aug[row][n] == 1) {
            return {}; // No solution
        }
    }
    
    // Back substitution - find solution with minimum clicks
    // We need to handle free variables properly
    // For simplicity, try all combinations of free variables (but N up to 200 is too large)
    // Instead, use the standard approach: set free variables to 0, compute pivot variables
    
    // Mark free variables
    vector<bool> is_free(n, true);
    for (int i = 0; i < m; i++) {
        if (pivot_col[i] != -1) {
            is_free[pivot_col[i]] = false;
        }
    }
    
    // Set free variables to 0 and compute pivot variables
    vector<int> solution(n, 0);
    for (int i = m - 1; i >= 0; i--) {
        if (pivot_col[i] == -1) continue;
        
        int col = pivot_col[i];
        int val = aug[i][n];
        for (int j = col + 1; j < n; j++) {
            val ^= (aug[i][j] & solution[j]);
        }
        solution[col] = val;
    }
    
    return solution;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    // Build adjacency matrix A: A[i][j] = 1 if clicking cell j affects cell i
    // Note: cell indices in input are 1-indexed
    vector<vector<int>> A(N, vector<int>(N, 0));
    
    for (int j = 0; j < N; j++) {
        int k;
        cin >> k;
        for (int idx = 0; idx < k; idx++) {
            int cell;
            cin >> cell;
            cell--; // Convert to 0-indexed
            A[cell][j] = 1; // clicking cell j+1 affects cell cell+1
        }
    }
    
    // Read initial state
    vector<int> state(N);
    for (int i = 0; i < N; i++) {
        cin >> state[i];
    }
    
    vector<int> best_solution;
    int min_clicks = INT_MAX;
    
    // Try to make all cells 0
    vector<int> solution0 = solveGF2(A, state);
    if (!solution0.empty()) {
        int clicks = 0;
        for (int x : solution0) clicks += x;
        if (clicks < min_clicks) {
            min_clicks = clicks;
            best_solution = solution0;
        }
    }
    
    // Try to make all cells 1 (target = state XOR ones)
    vector<int> target_ones(N);
    for (int i = 0; i < N; i++) {
        target_ones[i] = 1 ^ state[i];
    }
    
    vector<int> solution1 = solveGF2(A, target_ones);
    if (!solution1.empty()) {
        int clicks = 0;
        for (int x : solution1) clicks += x;
        if (clicks < min_clicks) {
            min_clicks = clicks;
            best_solution = solution1;
        }
    }
    
    if (min_clicks == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << min_clicks << endl;
        if (min_clicks > 0) {
            vector<int> clicks_list;
            for (int i = 0; i < N; i++) {
                if (best_solution[i] == 1) {
                    clicks_list.push_back(i + 1); // Convert to 1-indexed
                }
            }
            for (int i = 0; i < clicks_list.size(); i++) {
                if (i > 0) cout << " ";
                cout << clicks_list[i];
            }
            cout << endl;
        }
    }
    
    return 0;
}
