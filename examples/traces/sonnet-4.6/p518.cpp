#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> l(n+1, vector<int>(n+1, 0));
    
    // The i-th number in the j-th line is lij
    // "The i-th number in the j-th line lij" — j-th line, i-th number = l[i][j]? 
    // Let me re-read: "i-th number in j-th line is lij = length between i-th and j-th"
    // So line j (1-indexed), position i gives l[i][j]
    // That means if we read row by row: row j, column i = l[i][j] = l[j][i] (symmetric)
    // Actually reading row j gives: l[1][j], l[2][j], ..., l[n][j]
    // But since symmetric, l[i][j] = l[j][i], so it's just a symmetric matrix.
    
    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= n; i++) {
            cin >> l[i][j];
        }
    }
    
    int k;
    cin >> k;
    vector<int> r(k+1);
    for (int i = 1; i <= k; i++) {
        cin >> r[i];
    }
    
    // dp[v] = reachable at current step
    vector<bool> dp(n+1, false);
    dp[1] = true;
    
    for (int step = 1; step <= k; step++) {
        vector<bool> ndp(n+1, false);
        for (int v = 1; v <= n; v++) {
            if (!dp[v]) continue;
            // move along edge of weight r[step]
            for (int u = 1; u <= n; u++) {
                if (l[v][u] == r[step]) {
                    ndp[u] = true;
                }
            }
        }
        dp = ndp;
    }
    
    vector<int> result;
    for (int v = 1; v <= n; v++) {
        if (dp[v]) result.push_back(v);
    }
    
    if (result.empty()) {
        cout << 0 << endl;
    } else {
        cout << result.size() << "\n";
        for (int i = 0; i < (int)result.size(); i++) {
            if (i > 0) cout << " ";
            cout << result[i];
        }
        cout << "\n";
    }
    
    return 0;
}
