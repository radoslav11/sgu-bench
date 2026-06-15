#include<bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    
    while(N--) {
        int P;
        cin >> P;
        
        int total = 2 * P;
        
        // dp[i][j][s] = number of ways to choose j items from first i items with sum % P == s
        vector<vector<vector<long long>>> dp(total + 1, vector<vector<long long>>(P + 1, vector<long long>(P, 0)));
        
        dp[0][0][0] = 1;
        
        for(int i = 1; i <= total; i++) {
            for(int j = 0; j <= P; j++) {
                for(int s = 0; s < P; s++) {
                    // Don't take ball i
                    dp[i][j][s] += dp[i-1][j][s];
                    
                    // Take ball i
                    if(j > 0) {
                        int prev_s = (s - i % P + P) % P;
                        dp[i][j][s] += dp[i-1][j-1][prev_s];
                    }
                }
            }
        }
        
        cout << dp[total][P][0] << "\n";
    }
    
    return 0;
}
