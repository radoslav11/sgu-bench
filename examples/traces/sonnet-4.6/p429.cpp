#include <bits/stdc++.h>
using namespace std;

int n, a[5];
map<tuple<int,int,int,int>, bool> memo;

// Returns true if current player wins
bool solve(int l, int r, int rl, int rr) {
    if (l > r) return false;
    auto key = make_tuple(l, r, rl, rr);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    
    bool win = false;
    // Take from left
    for (int k = 1; k <= rl && !win; k++) {
        if (k == rl) {
            // Remove heap l
            if (l == r) win = !solve(l+1, r-1, 0, 0); // empty
            else win = !solve(l+1, r, a[l+1], rr);
        } else {
            win = !solve(l, r, rl-k, rr);
        }
    }
    // Take from right (only if l != r)
    if (l != r) {
        for (int k = 1; k <= rr && !win; k++) {
            if (k == rr) win = !solve(l, r-1, rl, a[r-1]);
            else win = !solve(l, r, rl, rr-k);
        }
    }
    
    memo[key] = win;
    return win;
}

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    cout << (solve(0, n-1, a[0], a[n-1]) ? "FEDOR" : "SERGEY") << endl;
}
