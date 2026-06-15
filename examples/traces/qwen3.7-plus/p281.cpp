#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> A(n), B(n);
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> B[i];
    }

    // Create a sorted list of unique team names to map them to integer IDs
    vector<string> names = A;
    sort(names.begin(), names.end());
    names.erase(unique(names.begin(), names.end()), names.end());

    vector<int> id_A(n), id_B(n);
    for (int i = 0; i < n; ++i) {
        id_A[i] = lower_bound(names.begin(), names.end(), A[i]) - names.begin();
        id_B[i] = lower_bound(names.begin(), names.end(), B[i]) - names.begin();
    }

    int mismatch = 0;
    vector<int> cnt(names.size(), 0);
    vector<string> current_block;

    // Process the ratings to find minimal blocks with identical sets of teams
    for (int i = 0; i < n; ++i) {
        int u = id_A[i];
        int v = id_B[i];

        // Update count for team from rating A
        if (cnt[u] == 0) mismatch++;
        cnt[u]++;
        if (cnt[u] == 0) mismatch--;

        // Update count for team from rating B
        if (cnt[v] == 0) mismatch++;
        cnt[v]--;
        if (cnt[v] == 0) mismatch--;

        current_block.push_back(A[i]);

        // When mismatch is 0, the sets of teams in the current block are identical
        if (mismatch == 0) {
            sort(current_block.begin(), current_block.end());
            for (const string& s : current_block) {
                cout << s << "\n";
            }
            current_block.clear();
        }
    }

    return 0;
}
