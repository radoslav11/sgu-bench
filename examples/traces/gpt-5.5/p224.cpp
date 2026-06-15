#include <bits/stdc++.h>
using namespace std;

int n, k;
long long ans = 0;

void dfs(int row, int placed, int cols, int diag1, int diag2) {
    if (placed == k) {
        ans++;
        return;
    }

    if (row == n) return;
    if (placed + (n - row) < k) return;

    // Option 1: do not place a queen in this row
    dfs(row + 1, placed, cols, diag1, diag2);

    // Option 2: place a queen in this row
    for (int col = 0; col < n; col++) {
        int cbit = 1 << col;
        int d1bit = 1 << (row - col + n - 1);
        int d2bit = 1 << (row + col);

        if ((cols & cbit) || (diag1 & d1bit) || (diag2 & d2bit))
            continue;

        dfs(row + 1, placed + 1, cols | cbit, diag1 | d1bit, diag2 | d2bit);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;

    if (k > n) {
        cout << 0 << '\n';
        return 0;
    }

    dfs(0, 0, 0, 0, 0);

    cout << ans << '\n';

    return 0;
}
