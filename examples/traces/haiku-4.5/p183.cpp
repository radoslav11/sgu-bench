#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    vector<int> C(N);
    for(int i = 0; i < N; i++){
        cin >> C[i];
    }
    
    const long long INF = 1e18;
    vector<long long> dp(N, INF);
    
    // dp[i] = min cost with last black ball at position i
    for(int i = 0; i < min(M, N); i++){
        dp[i] = C[i];
    }
    
    for(int i = 1; i < N; i++){
        for(int j = max(0, i - M); j < i; j++){
            if(dp[j] < INF){
                dp[i] = min(dp[i], dp[j] + C[i]);
            }
        }
    }
    
    long long ans = INF;
    for(int i = max(0, N - M); i < N; i++){
        ans = min(ans, dp[i]);
    }
    
    cout << ans << endl;
    
    return 0;
}
