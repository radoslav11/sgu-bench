#include <bits/stdc++.h>
using namespace std;

int n, k;
vector<int> validMasks;

bool isSingleRowValid(int mask) {
    return (mask & (mask << 1)) == 0;
}

int popcount(int mask) {
    return __builtin_popcount(mask);
}

bool areCompatible(int mask1, int mask2) {
    if (mask1 & mask2) return false;
    if (mask1 & (mask2 << 1)) return false;
    if (mask1 & (mask2 >> 1)) return false;
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> k;
    
    for (int mask = 0; mask < (1 << n); mask++) {
        if (isSingleRowValid(mask)) {
            validMasks.push_back(mask);
        }
    }
    
    // dp[row][mask][count] = number of ways
    vector<vector<vector<long long>>> dp(n, vector<vector<long long>>(1 << n, vector<long long>(k + 1, 0)));
    
    for (int mask : validMasks) {
        int cnt = popcount(mask);
        if (cnt <= k) {
            dp[0][mask][cnt] = 1;
        }
    }
    
    for (int row = 0; row < n - 1; row++) {
        for (int prevMask : validMasks) {
            for (int prevCnt = 0; prevCnt <= k; prevCnt++) {
                if (dp[row][prevMask][prevCnt] == 0) continue;
                
                for (int nextMask : validMasks) {
                    if (!areCompatible(prevMask, nextMask)) continue;
                    
                    int nextCnt = prevCnt + popcount(nextMask);
                    if (nextCnt <= k) {
                        dp[row + 1][nextMask][nextCnt] += dp[row][prevMask][prevCnt];
                    }
                }
            }
        }
    }
    
    long long answer = 0;
    for (int mask : validMasks) {
        answer += dp[n - 1][mask][k];
    }
    
    cout << answer << endl;
    
    return 0;
}
