#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    double t;
    cin >> n >> t;
    
    vector<double> p(n+1);
    for(int i = 1; i <= n; i++){
        cin >> p[i];
    }
    
    vector<double> dp(n+1, 1e18);
    dp[0] = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = i+1; j <= n; j++){
            double cost = (j - i) + t;
            double prob_ok = 1.0;
            double error_cost = 0.0;
            
            for(int k = i+1; k <= j; k++){
                double prob_error_at_k = p[k] * prob_ok;
                error_cost += prob_error_at_k * (j - k + dp[k-1]);
                prob_ok *= (1.0 - p[k]);
            }
            
            dp[j] = min(dp[j], dp[i] + cost + error_cost);
        }
    }
    
    cout << fixed << setprecision(12) << dp[n] << endl;
    
    return 0;
}
