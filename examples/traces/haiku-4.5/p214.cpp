#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string sigma, lambda, mu;
    getline(cin, sigma);
    getline(cin, lambda);
    getline(cin, mu);
    
    int n_sigma = sigma.length();
    vector<vector<int>> d(n_sigma, vector<int>(n_sigma));
    map<char, int> char_idx;
    
    for(int i = 0; i < n_sigma; i++){
        char_idx[sigma[i]] = i;
        for(int j = 0; j < n_sigma; j++){
            cin >> d[i][j];
        }
    }
    
    int n = lambda.length();
    int m = mu.length();
    
    // dp[i][j] = {min_cost, length}
    vector<vector<pair<int, int>>> dp(n+1, vector<pair<int, int>>(m+1, {INT_MAX, 0}));
    vector<vector<pair<int,int>>> parent(n+1, vector<pair<int,int>>(m+1, {-1,-1}));
    vector<vector<int>> choice(n+1, vector<int>(m+1, -1)); // 0: add lambda[i], 1: add mu[j], 2: add both
    
    dp[0][0] = {0, 0};
    
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= m; j++){
            if(dp[i][j].first == INT_MAX) continue;
            
            int cost = dp[i][j].first;
            int len = dp[i][j].second;
            
            // Add lambda[i] and any char from mu
            if(i < n){
                for(int c = 0; c < n_sigma; c++){
                    int new_cost = cost + d[char_idx[lambda[i]]][c];
                    if(new_cost < dp[i+1][j].first || (new_cost == dp[i+1][j].first && len+1 < dp[i+1][j].second)){
                        dp[i+1][j] = {new_cost, len+1};
                        parent[i+1][j] = {i, j};
                        choice[i+1][j] = c;
                    }
                }
            }
            
            // Add any char from alpha and mu[j]
            if(j < m){
                for(int c = 0; c < n_sigma; c++){
                    int new_cost = cost + d[c][char_idx[mu[j]]];
                    if(new_cost < dp[i][j+1].first || (new_cost == dp[i][j+1].first && len+1 < dp[i][j+1].second)){
                        dp[i][j+1] = {new_cost, len+1};
                        parent[i][j+1] = {i, j};
                        choice[i][j+1] = n_sigma + c;
                    }
                }
            }
            
            // Add both lambda[i] and mu[j]
            if(i < n && j < m){
                int new_cost = cost + d[char_idx[lambda[i]]][char_idx[mu[j]]];
                if(new_cost < dp[i+1][j+1].first || (new_cost == dp[i+1][j+1].first && len+1 < dp[i+1][j+1].second)){
                    dp[i+1][j+1] = {new_cost, len+1};
                    parent[i+1][j+1] = {i, j};
                    choice[i+1][j+1] = 2*n_sigma;
                }
            }
        }
    }
    
    cout << dp[n][m].first << "\n";
    
    string alpha = "", beta = "";
    int i = n, j = m;
    while(i > 0 || j > 0){
        int ch = choice[i][j];
        if(ch < n_sigma){
            alpha += lambda[i-1];
            beta += sigma[ch];
            i--; j--;
        } else if(ch < 2*n_sigma){
            alpha += sigma[ch - n_sigma];
            beta += mu[j-1];
            j--;
        } else {
            alpha += lambda[i-1];
            beta += mu[j-1];
            i--; j--;
        }
    }
    
    reverse(alpha.begin(), alpha.end());
    reverse(beta.begin(), beta.end());
    
    cout << alpha << "\n" << beta << "\n";
    
    return 0;
}
