#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9+9;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    cin >> n >> k;
    if((n+k)%2 != 0){ cout << 0; return 0; }
    int W = (n+k)/2;
    if(W > n){ cout << 0; return 0; }
    
    vector<pair<int,int>> players;
    for(int i=0;i<n;i++){int x;cin>>x;players.push_back({x,0});}
    for(int i=0;i<n;i++){int x;cin>>x;players.push_back({x,1});}
    sort(players.begin(), players.end());
    
    // dp[fB][wins]: fB = number of free Berland players
    // fY = fB - delta where delta = bseen - yseen (derived from context)
    vector<vector<long long>> dp(n+1, vector<long long>(W+1, 0));
    dp[0][0] = 1;
    
    int bseen = 0, yseen = 0;
    
    for(auto& [lv, team] : players){
        int delta = bseen - yseen;
        vector<vector<long long>> ndp(n+1, vector<long long>(W+1, 0));
        
        if(team == 0){  // Berland
            for(int fB = 0; fB <= n; fB++){
                int fY = fB - delta;
                if(fY < 0 || fY > n) continue;
                for(int w = 0; w <= W; w++){
                    long long v = dp[fB][w];
                    if(!v) continue;
                    // Option 1: Defer B (add to free pool)
                    if(fB+1 <= n)
                        ndp[fB+1][w] = (ndp[fB+1][w] + v) % MOD;
                    // Option 2: Match B with a free Y (B wins)
                    if(fY > 0 && w+1 <= W)
                        ndp[fB][w+1] = (ndp[fB][w+1] + v * fY) % MOD;
                }
            }
            bseen++;
        } else {  // Byteland
            for(int fB = 0; fB <= n; fB++){
                int fY = fB - delta;
                if(fY < 0 || fY > n) continue;
                for(int w = 0; w <= W; w++){
                    long long v = dp[fB][w];
                    if(!v) continue;
                    // Option 1: Defer Y (add to free pool)
                    ndp[fB][w] = (ndp[fB][w] + v) % MOD;
                    // Option 2: Match Y with a free B (Y wins)
                    if(fB > 0)
                        ndp[fB-1][w] = (ndp[fB-1][w] + v * fB) % MOD;
                }
            }
            yseen++;
        }
        
        dp = move(ndp);
    }
    
    cout << dp[0][W] % MOD << endl;
    return 0;
}
