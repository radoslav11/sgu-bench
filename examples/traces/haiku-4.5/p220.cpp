#include<bits/stdc++.h>
using namespace std;

long long solve(vector<vector<int>>& cells, int k) {
    if (k == 0) return 1;
    if (cells.empty()) return 0;
    
    int n = cells.size();
    
    // dp[i][j] = ways to place j bishops using first i diagonals
    map<pair<int,int>, long long> memo;
    
    function<long long(int, int, set<pair<int,int>>&)> backtrack = 
        [&](int idx, int placed, set<pair<int,int>>& diag) -> long long {
        if (placed == k) return 1;
        if (idx == n) return 0;
        
        long long res = backtrack(idx + 1, placed, diag); // skip this cell
        
        // try to place bishop at cells[idx]
        int r = cells[idx][0], c = cells[idx][1];
        int d1 = r - c;  // one diagonal direction
        int d2 = r + c;  // other diagonal direction
        
        if (diag.find({0, d1}) == diag.end() && diag.find({1, d2}) == diag.end()) {
            diag.insert({0, d1});
            diag.insert({1, d2});
            res += backtrack(idx + 1, placed + 1, diag);
            diag.erase({0, d1});
            diag.erase({1, d2});
        }
        
        return res;
    };
    
    set<pair<int,int>> diag;
    return backtrack(0, 0, diag);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k;
    cin >> n >> k;
    
    // Separate white and black squares
    vector<vector<int>> white, black;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i + j) % 2 == 0) {
                white.push_back({i, j});
            } else {
                black.push_back({i, j});
            }
        }
    }
    
    long long ans = 0;
    
    // Try all distributions of k bishops between white and black
    for (int w = 0; w <= k; w++) {
        int b = k - w;
        if (b > black.size()) continue;
        if (w > white.size()) continue;
        
        ans += solve(white, w) * solve(black, b);
    }
    
    cout << ans << endl;
    
    return 0;
}
