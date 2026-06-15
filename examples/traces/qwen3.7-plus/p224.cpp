#include <iostream>

using namespace std;

int n, k;
long long ans = 0;

void solve(int row, int count, int col_mask, int diag1_mask, int diag2_mask) {
    if (count == k) {
        ans++;
        return;
    }
    if (row == n) return;
    if (n - row < k - count) return;

    // Option 1: do not place a queen in this row
    solve(row + 1, count, col_mask, diag1_mask, diag2_mask);

    // Option 2: place a queen in this row
    for (int c = 0; c < n; ++c) {
        if ((col_mask & (1 << c)) == 0 &&
            (diag1_mask & (1 << (row - c + n))) == 0 &&
            (diag2_mask & (1 << (row + c))) == 0) {
            solve(row + 1, count + 1, 
                  col_mask | (1 << c),
                  diag1_mask | (1 << (row - c + n)),
                  diag2_mask | (1 << (row + c)));
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (cin >> n >> k) {
        if (k > n) {
            cout << 0 << "\n";
        } else {
            solve(0, 0, 0, 0, 0);
            cout << ans << "\n";
        }
    }
    return 0;
}
