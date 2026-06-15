#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    if (cin >> n >> k) {
        if (k == 0) {
            cout << (n == 0 ? 1 : 0) << "\n";
            return 0;
        }
        if (k > n) {
            cout << 0 << "\n";
            return 0;
        }
        
        // dp[j] will store the number of ways to partition i elements into j non-empty subsets
        vector<int> dp(k + 1, 0);
        dp[0] = 1;
        
        for (int i = 1; i <= n; ++i) {
            int limit = min(i, k);
            // Traverse backwards to use values from the previous iteration (i - 1)
            for (int j = limit; j >= 1; --j) {
                dp[j] = (dp[j - 1] + j * dp[j]) % 2007;
            }
            dp[0] = 0; // For i >= 1, there are 0 ways to partition into 0 subsets
        }
        
        cout << dp[k] << "\n";
    }
    
    return 0;
}
