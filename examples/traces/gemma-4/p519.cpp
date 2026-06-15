#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: 3D City Model
 * The goal is to calculate the total surface area of a 3D model made of cubes.
 * The model is represented as an n x m grid of building heights.
 * Each cell (i, j) with height H[i][j] > 0 contributes:
 *   - Top and bottom surface area: 2 (if H[i][j] > 0).
 *   - Exposed side areas: For each of its four sides, the area exposed to the 
 *     neighbor is max(0, current_height - neighbor_height). 
 *     If the building is on the boundary, the neighbor's height is treated as 0.
 *
 * Complexity:
 *   Time: O(n * m), where n and m are grid dimensions (up to 100).
 *   Space: O(n * m) to store the grid.
 */

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    // Read the number of rows (n) and columns (m)
    if (!(cin >> n >> m)) {
        return 0;
    }

    // The grid contains n lines, each with m digits representing heights.
    // We use a vector of strings to store the rows.
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> grid[i])) break;
    }

    long long total_area = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // Convert the character digit to an integer height
            int h = grid[i][j] - '0';

            // If height is 0, there's no building and no surface area contribution.
            if (h > 0) {
                // Top and bottom facets of the building
                total_area += 2;

                // Side area calculation:
                // We calculate how much of the current building's wall is exposed
                // compared to its neighbor in each of the four directions.

                // Upward direction
                int up_h = (i > 0) ? (grid[i - 1][j] - '0') : 0;
                total_area += max(0, h - up_h);

                // Downward direction
                int down_h = (i < n - 1) ? (grid[i + 1][j] - '0') : 0;
                total_area += max(0, h - down_h);

                // Leftward direction
                int left_h = (j > 0) ? (grid[i][j - 1] - '0') : 0;
                total_area += max(0, h - left_h);

                // Rightward direction
                int right_h = (j < m - 1) ? (grid[i][j + 1] - '0') : 0;
                total_area += max(0, h - right_h);
            }
        }
    }

    // Output the final calculated surface area.
    cout << total_area << endl;

    return 0;
}

