#include <iostream>
#include <vector>
#include <string>

/**
 * Problem 484: Kola
 * The problem asks us to simulate the movement of a bottle in a vending machine grid.
 * 
 * Rules of motion:
 * 1. The bottle starts at position 'P'.
 * 2. If the current cell is empty ('.' or 'P'), the bottle falls vertically down to the next row.
 * 3. If the current cell is an obstacle '/':
 *    - The bottle tries to move left in the same row.
 *    - If the cell to the left is a wall (out of bounds) or a different obstacle type ('\'), the bottle stops (-1).
 *    - If the cell to the left is the same obstacle ('/'), it continues moving left in the same row.
 *    - If the cell to the left is empty ('.'), it stops moving left and will fall to the next row in the next step.
 * 4. If the current cell is an obstacle '\':
 *    - The bottle tries to move right in the same row.
 *    - If the cell to the right is a wall or a different obstacle type ('/'), the bottle stops (-1).
 *    - If the cell to the right is the same obstacle ('\'), it continues moving right in the same row.
 *    - If the cell to the right is empty ('.'), it stops moving right and will fall to the next row in the next step.
 * 5. If the bottle falls from the last row, it reaches the tray.
 */

using namespace std;

int main() {
    // Optimize standard input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<string> grid(n);
    int start_r = -1, start_c = -1;

    // Read grid and find the starting position 'P'
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'P') {
                start_r = i;
                start_c = j;
            }
        }
    }

    int r = start_r, c = start_c;

    // Simulation loop
    // The bottle is "in" the current cell (r, c).
    // r < n ensures we are within the rows of the vending machine.
    while (r < n) {
        if (grid[r][c] == '.' || grid[r][c] == 'P') {
            // Empty cell: falls down to the next row.
            r++;
        } else if (grid[r][c] == '/') {
            // Obstacle '/': move left.
            int next_c = c - 1;
            // If it hits a wall or a different obstacle, it stops.
            if (next_c < 0 || grid[r][next_c] == '\\') {
                cout << -1 << endl;
                return 0;
            }
            // Otherwise, update the current column.
            // If grid[r][next_c] is '/', the next iteration will process the same row.
            // If grid[r][next_c] is '.', the next iteration will fall to the next row.
            c = next_c;
        } else if (grid[r][c] == '\\') {
            // Obstacle '\': move right.
            int next_c = c + 1;
            // If it hits a wall or a different obstacle, it stops.
            if (next_c >= m || grid[r][next_c] == '/') {
                cout << -1 << endl;
                return 0;
            }
            // Otherwise, update the current column.
            c = next_c;
        } else {
            // Unexpected character in the input.
            cout << -1 << endl;
            return 0;
        }
    }

    // If r reaches n, the bottle has fallen from the bottom row into the tray.
    // Output the 1-indexed column number.
    cout << c + 1 << endl;

    return 0;
}

