#include<bits/stdc++.h>
using namespace std;

int main(){
    int S;
    cin >> S;
    
    long long dp[21][10][2];
    memset(dp, 0, sizeof(dp));
    
    for(int d = 0; d < 10; d++){
        dp[1][d][0] = 1;
    }
    
    for(int i = 1; i < S; i++){
        for(int d = 0; d < 10; d++){
            if(dp[i][d][0] > 0){
                for(int nd = d; nd < 10; nd++){
                    dp[i+1][nd][0] += dp[i][d][0];
                }
                for(int nd = 0; nd <= d; nd++){
                    dp[i+1][nd][1] += dp[i][d][0];
                }
            }
            if(dp[i][d][1] > 0){
                for(int nd = 0; nd <= d; nd++){
                    dp[i+1][nd][1] += dp[i][d][1];
                }
            }
        }
    }
    
    long long answer = 0;
    for(int d = 0; d < 10; d++){
        answer += dp[S][d][0] + dp[S][d][1];
    }
    
    cout << answer << endl;
    
    return 0;
}
