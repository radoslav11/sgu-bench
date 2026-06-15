#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    // If k is 0, there's exactly 1 way (empty board)
    if (k == 0) {
        cout << 1 << endl;
        return 0;
    }

    // Generate all valid masks for a single row (no adjacent kings)
    vector<int> validMasks;
    int maxMask = 1 << n;
    for (int mask = 0; mask < maxMask; mask++) {
        // Check no two adjacent bits are set
        if ((mask & (mask << 1)) == 0) {
            validMasks.push_back(mask);
        }
    }

    // dp[mask][count] = number of ways to place 'count' kings ending with this mask
    // Use vector of maps for better memory efficiency
    vector<vector<long long>> dp(maxMask, vector<long long>(k + 1, 0));
    
    // Initialize first row
    for (int mask : validMasks) {
        int cnt = __builtin_popcount(mask);
        if (cnt <= k) {
            dp[mask][cnt] = 1;
        }
    }

    // Process remaining rows
    for (int row = 1; row < n; row++) {
        vector<vector<long long>> new_dp(maxMask, vector<long long>(k + 1, 0));
        
        for (int prevMask : validMasks) {
            for (int prevCount = 0; prevCount <= k; prevCount++) {
                if (dp[prevMask][prevCount] == 0) continue;
                
                for (int currMask : validMasks) {
                    // Check if current mask is compatible with previous mask
                    // No vertical or diagonal adjacency
                    
                    // Check vertical adjacency (same column)
                    if (currMask & prevMask) continue;
                    
                    // Check diagonal adjacency
                    if ((currMask & (prevMask << 1)) || (currMask & (prevMask >> 1))) continue;
                    
                    int currCount = __builtin_popcount(currMask);
                    int newCount = prevCount + currCount;
                    if (newCount <= k) {
                        new_dp[currMask][newCount] += dp[prevMask][prevCount];
                    }
                }
            }
        }
        
        dp = move(new_dp);
    }

    // Sum all configurations with exactly k kings
    long long result = 0;
    for (int mask = 0; mask < maxMask; mask++) {
        if (mask < (int)dp.size() && k < (int)dp[0].size()) {
            result += dp[mask][k];
        }
    }
    
    cout << result << endl;
    
    return 0;
}
