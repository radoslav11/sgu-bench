#include <bits/stdc++.h>
using namespace std;
typedef vector<vector<long long>> VVL;

int main(){
    int n, k;
    cin >> n >> k;
    
    // For each color (0 and 1), get sorted diagonal lengths
    // "/" diagonal: row+col = s, color = s%2
    // length of diagonal s in n×n board: n - abs(n-1-s)
    // but only squares of color s%2
    // each square on diagonal s has row+col=s, so color=s%2, length = n-abs(n-1-s)
    // but we want number of squares of that color on diagonal s
    // all squares on diagonal s have same color s%2, so length is as computed
    
    auto solve = [&](int color) -> vector<long long> {
        vector<int> lens;
        for(int s = color; s <= 2*(n-1); s += 2){
            int len = n - abs(n-1-s);
            lens.push_back(len);
        }
        int m = lens.size();
        // dp[j] = ways to place j bishops on first i diagonals
        vector<long long> dp(m+1, 0);
        dp[0] = 1;
        for(int i = 0; i < m; i++){
            int L = lens[i];
            for(int j = min(i+1, (int)dp.size()-1); j >= 1; j--){
                dp[j] += dp[j-1] * (long long)(L - (j-1));
            }
        }
        return dp;
    };
    
    auto d0 = solve(0);
    auto d1 = solve(1);
    
    long long ans = 0;
    for(int j = 0; j <= k; j++){
        int j2 = k - j;
        long long v0 = (j < (int)d0.size()) ? d0[j] : 0;
        long long v1 = (j2 < (int)d1.size()) ? d1[j2] : 0;
        ans += v0 * v1;
    }
    
    cout << ans << endl;
}
