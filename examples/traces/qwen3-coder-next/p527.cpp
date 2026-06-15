#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> col_mask(n, 0);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < m; j++) {
            if (s[j] == '*') {
                col_mask[i] |= (1 << j);
            }
        }
    }

    int min_bombs = n + m; // worst case: n rows or m columns, but actually max(n, m) <= 25, so n+m is safe
    // Iterate over all subsets of rows to cover (R)
    for (int mask = 0; mask < (1 << n); mask++) {
        int r = __builtin_popcount(mask);
        int col_covered = 0;
        // For rows not covered by the current mask (i.e., not in R), OR their column masks
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) {
                col_covered |= col_mask[i];
            }
        }
        int c = __builtin_popcount(col_covered);
        min_bombs = min(min_bombs, max(r, c));
    }

    cout << min_bombs << '\n';

    return 0;
}
