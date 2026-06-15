#include <iostream>
#include <vector>

/**
 * Problem 362. Robot-Annihilator
 * 
 * The robot is on an N x M factory rectangle.
 * The robot's movement algorithm:
 * 1. At (i, j), put a bomb.
 * 2. Choose the first existing, non-exploded neighbor from the list:
 *    (i + 1, j) [Down], (i, j - 1) [Left], (i - 1, j) [Up], (i, j + 1) [Right].
 * 3. If a neighbor is chosen, move to it. Once it leaves (i, j), (i, j) explodes.
 * 4. If no valid neighbor is found, the robot stays and explodes.
 * 
 * Constraints: 1 <= N, M <= 10. Small enough for simulation.
 */

using namespace std;

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    int is, js;
    if (!(cin >> is >> js)) return 0;

    // exploded[r][c] tracks whether building (r, c) has exploded.
    // Using 1-based indexing for convenience.
    bool exploded[12][12];
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            exploded[i][j] = false;
        }
    }

    // Directions list in the order specified: 
    // (i+1, j) [Down], (i, j-1) [Left], (i-1, j) [Up], (i, j+1) [Right]
    int dr[] = {1, 0, -1, 0};
    int dc[] = {0, -1, 0, 1};
    char move_chars[] = {'D', 'L', 'U', 'R'};

    int curr_r = is;
    int curr_c = js;

    // The simulation continues as long as the robot can move to a neighbor.
    while (true) {
        int next_r = -1;
        int next_c = -1;
        char move_char = ' ';

        // Find the first existing and non-exploded neighbor.
        for (int i = 0; i < 4; ++i) {
            int nr = curr_r + dr[i];
            int nc = curr_c + dc[i];

            // Check if the building exists (within bounds N x M)
            // and if it hasn't exploded yet.
            if (nr >= 1 && nr <= N && nc >= 1 && nc <= M && !exploded[nr][nc]) {
                next_r = nr;
                next_c = nc;
                move_char = move_chars[i];
                break; // Take the first available direction from the priority list.
            }
        }

        // If no valid neighbor is found, the robot explodes at the current building.
        if (next_r == -1) {
            break;
        }

        // The robot moves to the chosen neighbor.
        // The building the robot leaves explodes immediately after it leaves.
        exploded[curr_r][curr_c] = true;
        curr_r = next_r;
        curr_c = next_c;

        // Print the movement character.
        cout << move_char;
    }

    cout << endl;

    return 0;
}

