#include <iostream>

using namespace std;

int main() {
    // Optimize standard I/O operations for speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int min_count = 1e9;
    int best_k = 0;

    // There are 3 possible diagonal patterns that satisfy the condition.
    // We check all 3 and pick the one with the minimum number of blue cells.
    for (int k = 0; k < 3; ++k) {
        int count = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if ((i + j) % 3 == k) {
                    count++;
                }
            }
        }
        if (count < min_count) {
            min_count = count;
            best_k = k;
        }
    }

    // Output the grid with the optimal pattern
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if ((i + j) % 3 == best_k) {
                cout << '#';
            } else {
                cout << '0';
            }
        }
        cout << "\n";
    }

    return 0;
}
