#include <bits/stdc++.h>
using namespace std;

bool canCombine(vector<int> positions, int n) {
    // BFS/DFS: try all folds
    // State: sorted unique positions + n
    set<pair<vector<int>,int>> visited;
    queue<pair<vector<int>,int>> q;
    sort(positions.begin(), positions.end());
    positions.erase(unique(positions.begin(), positions.end()), positions.end());
    q.push({positions, n});
    visited.insert({positions, n});
    while (!q.empty()) {
        auto [pos, nn] = q.front(); q.pop();
        if (pos.size() == 1) return true;
        // try all folds 1..nn-1
        for (int f = 1; f < nn; f++) {
            vector<int> np;
            int newn = max(f, nn - f);
            bool valid = true;
            for (int p : pos) {
                int mapped = (p <= f) ? p : 2*f+1-p;
                if (mapped < 1 || mapped > newn) { valid = false; break; }
                np.push_back(mapped);
            }
            if (!valid) continue;
            sort(np.begin(), np.end());
            np.erase(unique(np.begin(), np.end()), np.end());
            if (!visited.count({np, newn})) {
                visited.insert({np, newn});
                q.push({np, newn});
            }
        }
    }
    return false;
}

int main(){
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> rows, cols;
    for (int i = 0; i < k; i++) {
        int x, y; cin >> x >> y;
        rows.push_back(x);
        cols.push_back(y);
    }
    if (canCombine(rows, n) && canCombine(cols, m))
        cout << "YES\n";
    else
        cout << "NO\n";
}
