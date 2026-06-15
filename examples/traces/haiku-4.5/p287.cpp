#include<bits/stdc++.h>
using namespace std;

int main(){
    long long q, c;
    cin >> q >> c;
    
    if(c == 0) {
        // Standard binary search
        long long ans = 0;
        long long temp = q;
        while(temp > 0) {
            temp /= 2;
            ans++;
        }
        cout << ans + 1 << endl;
        return 0;
    }
    
    // For c > 0, we use dynamic programming/simulation
    // dp[i] = max q that can be solved in i steps with delay c
    vector<long long> dp(5000001, 0);
    dp[0] = 1;
    
    for(int i = 1; i <= 5000000; i++) {
        if(i <= c) {
            dp[i] = 1; // Can't get useful info in first c steps
        } else {
            // After c steps, we can use binary search on previous answer
            dp[i] = dp[i - c - 1] + dp[i - c] + 1;
            if(dp[i] > q) {
                cout << i << endl;
                return 0;
            }
        }
    }
    
    return 0;
}
