#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * The goal is to find a subset of dragons to kill such that:
 * 1. For every princess at cell i (where 2 <= i < n), the number of dragons killed
 *    in cells [2, i-1] is strictly less than the princess's beauty b_i.
 * 2. For the princess at the last cell n, the number of dragons killed in cells
 *    [2, n-1] is greater than or equal to her beauty b_n.
 * 3. The total gold collected (sum of g_i for all killed dragons) is maximized.
 * 
 * Greedy Approach:
 * Process the cells from left to right (from 2 to n).
 * - If the current cell contains a dragon, we tentatively kill it. We add its gold
 *   to the total and keep track of its index in a min-priority queue.
 * - If the current cell contains a princess (and it's not the last cell), we check
 *   if the current count of killed dragons is greater than or equal to her beauty.
 *   If it is, we must "undo" some kills to satisfy the condition. To maximize
 *   the gold, we always remove the dragon with the smallest amount of gold among
 *   the dragons killed so far. This also keeps the count of killed dragons as large
 *   as possible for future requirements.
 * - If the current cell is the last cell (which is always a princess), we check if
 *   the current count of killed dragons is at least her beauty b_n. If not, it's
 *   impossible to marry her, so output -1.
 * 
 * Complexity:
 * - Time: O(n log n) due to the priority queue operations.
 * - Space: O(n) to store the cells and the killed dragon indices.
 */

struct Cell {
    char type;
    int value;
};

int main() {
    // Fast input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // Cells are numbered from 1 to n, but the input provides descriptions for cells 2 to n.
    // We use 1-based indexing for simplicity.
    vector<Cell> cells(n + 1);
    for (int i = 2; i <= n; ++i) {
        cin >> cells[i].type >> cells[i].value;
    }

    // A min-priority queue to store (gold, cell_index) of the dragons killed so far.
    // Using a min-priority queue allows us to efficiently find and remove the dragon
    // with the minimum gold whenever we hit a princess's beauty limit.
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    long long total_gold = 0;
    int killed_count = 0;

    for (int i = 2; i <= n; ++i) {
        if (cells[i].type == 'd') {
            // Encountered a dragon: kill it and add to our gold total.
            pq.push({cells[i].value, i});
            total_gold += cells[i].value;
            killed_count++;
        } else {
            // Encountered a princess.
            if (i < n) {
                // For princesses before the destination, we must satisfy count < b_i.
                // If the current count is too high, remove the smallest gold dragons.
                while (killed_count >= cells[i].value) {
                    pair<int, int> smallest_dragon = pq.top();
                    pq.pop();
                    total_gold -= smallest_dragon.first;
                    killed_count--;
                }
            } else {
                // For the princess at the destination (cell n), we must satisfy count >= b_n.
                if (killed_count < cells[i].value) {
                    cout << -1 << endl;
                    return 0;
                }
            }
        }
    }

    // The greedy approach ensures we have the maximum possible number of dragons
    // that satisfy the beauty constraints, and for that count, the maximum gold.
    // Since every dragon has gold >= 1, more dragons always mean more gold.
    // Thus, we should output all dragons currently in the priority queue.

    cout << total_gold << "\n";
    cout << killed_count << "\n";

    vector<int> result_indices;
    while (!pq.empty()) {
        result_indices.push_back(pq.top().second);
        pq.pop();
    }
    
    // The problem asks for dragon indices in increasing order.
    sort(result_indices.begin(), result_indices.end());

    for (int i = 0; i < (int)result_indices.size(); ++i) {
        cout << result_indices[i] << (i == (int)result_indices.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

