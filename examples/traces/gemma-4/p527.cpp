#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * We want to find the minimum number of bombs to clear all rocks ('*') in an n x m grid.
 * A bomb at (i, j) destroys all rocks in row i and all rocks in column j.
 * This problem can be framed as choosing a set of rows R and a set of columns C 
 * such that all rocks are covered, and the value max(|R|, |C|) is minimized.
 * 
 * Let n be the number of rows and m be the number of columns.
 * Let R be a subset of rows and C be a subset of columns.
 * A rock at (i, j) is covered if i is in R or j is in C.
 * For a fixed set of rows R, the minimal set of columns C needed to cover all 
 * remaining rocks (those not in the chosen rows) consists of all columns j 
 * that contain at least one rock whose row index i is not in R.
 * 
 * Let col_mask[j] be the bitmask of rows containing rocks in column j.
 * A column j's rocks are entirely contained in R if and only if col_mask[j] 
 * is a subset of the bitmask representing R.
 * 
 * If we define g(R) as the number of columns j such that col_mask[j] ⊆ R,
 * then the number of columns that MUST be included in C is m - g(R).
 * We want to minimize max(|R|, m - g(R)) over all subsets R.
 * 
 * Given n, m <= 25, we can use SOS DP (Sum Over Subsets) to compute g(R) for all 
 * 2^n possible row subsets R in O(n * 2^n) time.
 */

// Using a static array to avoid stack overflow. 
// For n = 25, 2^25 elements takes 128 MiB.
int count_arr[1 << 25];

int main() {
    // Optimization for faster I/O operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }

    // To minimize complexity (2^n), ensure n is the smaller dimension.
    if (n > m) {
        vector<string> new_grid(m, string(n, '.'));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                new_grid[j][i] = grid[i][j];
            }
        }
        grid = new_grid;
        swap(n, m);
    }

    // At this point, n is the smaller dimension and n <= 25.
    // Step 1: Create a bitmask for each column representing its rocks' row indices.
    for (int j = 0; j < m; ++j) {
        int col_mask = 0;
        for (int i = 0; i < n; ++i) {
            if (grid[i][j] == '*') {
                col_mask |= (1 << i);
            }
        }
        // count_arr[mask] initially stores how many columns have that specific rock mask.
        count_arr[col_mask]++;
    }

    // Step 2: Perform Sum Over Subsets (SOS) DP.
    // This calculates for each mask: count_arr[mask] = number of columns j such that col_mask[j] ⊆ mask.
    // Using an optimized SOS DP loop that runs in O(n * 2^(n-1)) additions.
    for (int i = 0; i < n; ++i) {
        int bit = 1 << i;
        for (int mask = 0; mask < (1 << n); mask += (bit << 1)) {
            for (int submask = 0; submask < bit; ++submask) {
                count_arr[mask | bit | submask] += count_arr[mask | submask];
            }
        }
    }

    // Step 3: Iterate through all possible subsets of rows R to find the minimum max(|R|, |C|).
    // Initialize min_bombs to a safe upper bound (min(n, m)).
    int min_bombs = min(n, m);
    for (int mask = 0; mask < (1 << n); ++mask) {
        int r = __builtin_popcount(mask);
        // The number of columns needing coverage is m minus columns already covered by rows in the mask.
        int c = m - count_arr[mask];
        int current_max = max(r, c);
        if (current_max < min_bombs) {
            min_bombs = current_max;
        }
    }

    cout << min_bombs << endl;

    return 0;
}

