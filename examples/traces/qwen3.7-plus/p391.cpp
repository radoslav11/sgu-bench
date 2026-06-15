#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

bool check_1d(const string& S) {
    int n = S.length();
    int total_M = 0;
    for (char c : S) {
        if (c == 'M') total_M++;
    }
    if (total_M == 0) return true;
    if (total_M == 1) return true;
    
    vector<vector<bool>> is_pal(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++) {
        is_pal[i][i] = true;
        if (i + 1 < n) {
            is_pal[i][i+1] = (S[i] == S[i+1]);
        }
    }
    for (int len = 3; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            is_pal[i][j] = (S[i] == S[j] && is_pal[i+1][j-1]);
        }
    }
    
    vector<int> pref_M(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref_M[i+1] = pref_M[i] + (S[i] == 'M' ? 1 : 0);
    }
    
    auto count_M = [&](int i, int j) {
        return pref_M[j+1] - pref_M[i];
    };
    
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    queue<pair<int, int>> q;
    
    q.push({0, n - 1});
    visited[0][n - 1] = true;
    
    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop();
        
        if (count_M(i, j) == 1) return true;
        
        int L = j - i + 1;
        
        // Rule 1: fold left onto right
        // Requires prefix of length 2*k to be a palindrome
        for (int k = 1; 2 * k <= L; k++) {
            if (is_pal[i][i + 2 * k - 1]) {
                int ni = i + k;
                int nj = j;
                if (!visited[ni][nj]) {
                    visited[ni][nj] = true;
                    q.push({ni, nj});
                }
            }
        }
        
        // Rule 2: fold right onto left
        // Requires suffix of length 2*m to be a palindrome
        for (int m = 1; 2 * m <= L; m++) {
            if (is_pal[j - 2 * m + 1][j]) {
                int ni = i;
                int nj = j - m;
                if (!visited[ni][nj]) {
                    visited[ni][nj] = true;
                    q.push({ni, nj});
                }
            }
        }
    }
    
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    
    set<pair<int, int>> pts;
    set<int> rows, cols;
    
    for (int i = 0; i < k; i++) {
        int r, c;
        cin >> r >> c;
        pts.insert({r, c});
        rows.insert(r);
        cols.insert(c);
    }
    
    if (pts.size() != (size_t)rows.size() * cols.size()) {
        cout << "NO\n";
        return 0;
    }
    
    string row_str = "";
    row_str.reserve(n);
    for (int i = 1; i <= n; i++) {
        if (rows.count(i)) row_str += "M";
        else row_str += "U";
    }
    
    string col_str = "";
    col_str.reserve(m);
    for (int i = 1; i <= m; i++) {
        if (cols.count(i)) col_str += "M";
        else col_str += "U";
    }
    
    if (check_1d(row_str) && check_1d(col_str)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
