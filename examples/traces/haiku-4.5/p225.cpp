#include <bits/stdc++.h>
using namespace std;

int n, k;
vector<pair<int, int>> knights;
long long result = 0;

// Knight move offsets
int dr[] = {2, 2, -2, -2, 1, 1, -1, -1};
int dc[] = {1, -1, 1, -1, 2, -2, 2, -2};

bool isValid(int r, int c) {
    return r >= 0 && r < n && c >= 0 && c < n;
}

bool isAttacking(int r1, int c1, int r2, int c2) {
    for (int i = 0; i < 8; i++) {
        if (r1 + dr[i] == r2 && c1 + dc[i] == c2) {
            return true;
        }
    }
    return false;
}

bool canPlace(int r, int c) {
    for (auto& knight : knights) {
        if (isAttacking(r, c, knight.first, knight.second)) {
            return false;
        }
    }
    return true;
}

void backtrack(int pos, int placed) {
    if (placed == k) {
        result++;
        return;
    }
    
    // If we can't place enough knights from remaining squares
    if (pos == n * n) {
        return;
    }
    
    // Pruning: if we don't have enough squares left
    if (n * n - pos < k - placed) {
        return;
    }
    
    int r = pos / n;
    int c = pos % n;
    
    // Try not placing a knight at this position
    backtrack(pos + 1, placed);
    
    // Try placing a knight at this position
    if (canPlace(r, c)) {
        knights.push_back({r, c});
        backtrack(pos + 1, placed + 1);
        knights.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> k;
    
    backtrack(0, 0);
    
    cout << result << endl;
    
    return 0;
}
