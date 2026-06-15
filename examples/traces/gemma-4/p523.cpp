#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem Analysis:
 * The elevator moves according to specific rules:
 * 1. It starts at floor f. A sequence of buttons e1, e2, ..., en is pressed.
 * 2. The initial target 'a' is the earliest pressed button currently highlighted.
 * 3. While moving towards 'a', if the elevator passes a highlighted floor 'b', 
 *    it stops at 'b', unhighlights it, and marks it visited. Then it continues 
 *    moving towards the original target 'a'.
 * 4. When it reaches the target 'a', it unhighlights 'a' and marks it visited.
 * 5. After reaching 'a', a new target is selected: the earliest pressed button 
 *    that is still highlighted.
 * 6. This process repeats until no highlighted buttons remain.
 * 
 * Constraints:
 * n, f <= 100, ei <= 100.
 * The algorithm should be efficient enough to handle the 0.25s time limit.
 * O(n * max_floor) is around 10,000 operations, well within the limit.
 */

int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, f;
    // Reading n (number of button presses) and f (initial floor)
    if (!(cin >> n >> f)) return 0;

    // e stores the buttons in the order they were pressed
    vector<int> e(n);
    // highlighted tracks which floor buttons are currently lit up
    vector<bool> highlighted(101, false);
    for (int i = 0; i < n; ++i) {
        cin >> e[i];
        highlighted[e[i]] = true;
    }

    // visited stores the floors where the elevator stops in order
    vector<int> visited;
    int current_floor = f;

    while (true) {
        int target = -1;
        // Step 2: Find the earliest pressed button that is still highlighted
        for (int i = 0; i < n; ++i) {
            if (highlighted[e[i]]) {
                target = e[i];
                break;
            }
        }

        // Step 6: If no more highlighted buttons are found, the process stops
        if (target == -1) break;

        // Step 3 & 4: Determine direction and simulate movement
        if (target > current_floor) {
            // Moving upwards
            for (int floor = current_floor + 1; floor <= target; ++floor) {
                if (highlighted[floor]) {
                    // Stop at highlighted floor
                    visited.push_back(floor);
                    highlighted[floor] = false;
                    current_floor = floor;
                    // Rule: "Then the elevator continues moving towards the floor a."
                    // The 'for' loop continues through the original range [initial_current_floor + 1, target].
                }
            }
        } else {
            // Moving downwards
            for (int floor = current_floor - 1; floor >= target; --floor) {
                if (highlighted[floor]) {
                    // Stop at highlighted floor
                    visited.push_back(floor);
                    highlighted[floor] = false;
                    current_floor = floor;
                    // Rule: "Then the elevator continues moving towards the floor a."
                }
            }
        }
    }

    // Output the results separated by space
    for (int i = 0; i < (int)visited.size(); ++i) {
        cout << visited[i] << (i == (int)visited.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

