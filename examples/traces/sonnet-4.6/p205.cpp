#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> x(n);
    for(int i=0;i<n;i++) cin>>x[i];
    int m, s;
    cin >> m >> s;
    vector<vector<int>> L(m, vector<int>(s));
    for(int i=0;i<m;i++)
        for(int j=0;j<s;j++)
            cin >> L[i][j];
    
    const int INF = 1e9;
    // dp[ns] = min cost, next set = ns
    vector<int> dp(m, INF);
    dp[0] = 0;
    // from[j][ns] = {prev_ns, k_j}
    vector<vector<pair<int,int>>> from(n, vector<pair<int,int>>(m, {-1,-1}));
    
    for(int j=0;j<n;j++){
        vector<int> ndp(m, INF);
        for(int cs=0;cs<m;cs++){
            if(dp[cs]==INF) continue;
            for(int k=0;k<s;k++){
                int cost = abs(x[j]-L[cs][k]);
                int ns = k & (m-1);
                if(dp[cs]+cost < ndp[ns]){
                    ndp[ns] = dp[cs]+cost;
                    from[j][ns] = {cs, k};
                }
            }
        }
        dp = ndp;
    }
    
    int best_ns = min_element(dp.begin(),dp.end())-dp.begin();
    cout << dp[best_ns] << "\n";
    
    vector<int> ans(n);
    int cur_ns = best_ns;
    for(int j=n-1;j>=0;j--){
        auto [prev_ns, k] = from[j][cur_ns];
        ans[j] = k;
        cur_ns = prev_ns;
    }
    for(int i=0;i<n;i++) cout << ans[i] << " \n"[i==n-1];
}
