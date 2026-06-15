#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<pair<int, int>> intervals(n);
    for (int i = 0; i < n; ++i) {
        cin >> intervals[i].first >> intervals[i].second;
    }

    // Sort intervals by A_k in ascending order.
    // Since all A_k are different, this strictly orders them by A_k.
    sort(intervals.begin(), intervals.end());

    int max_b = -1;
    int redundant_count = 0;

    for (int i = 0; i < n; ++i) {
        // Because the array is sorted by A, for any j < i, A_j < A_i is guaranteed.
        // We just need to check if there is any j < i such that B_j > B_i.
        // This is equivalent to checking if B_i is less than the maximum B seen so far.
        if (intervals[i].second < max_b) {
            redundant_count++;
        } else {
            max_b = intervals[i].second;
        }
    }

    cout << redundant_count << "\n";

    return 0;
}
