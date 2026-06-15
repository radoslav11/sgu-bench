#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<char> type(n+1);
    vector<int> value(n+1);
    
    for(int i = 2; i <= n; i++){
        cin >> type[i] >> value[i];
    }
    
    // dp[i][j] = max gold to reach cell i having killed j dragons, -1 if impossible
    // j can be up to n-1 (number of dragons)
    vector<vector<long long>> dp(n+1, vector<long long>(n, -1));
    vector<vector<vector<int>>> path(n+1, vector<vector<int>>(n));
    
    dp[1][0] = 0;
    
    for(int i = 1; i < n; i++){
        for(int j = 0; j < n; j++){
            if(dp[i][j] == -1) continue;
            
            if(type[i+1] == 'd'){
                // Option 1: skip dragon
                if(dp[i+1][j] < dp[i][j]){
                    dp[i+1][j] = dp[i][j];
                    path[i+1][j] = path[i][j];
                }
                
                // Option 2: kill dragon
                if(j+1 < n){
                    if(dp[i+1][j+1] < dp[i][j] + value[i+1]){
                        dp[i+1][j+1] = dp[i][j] + value[i+1];
                        path[i+1][j+1] = path[i][j];
                        path[i+1][j+1].push_back(i+1);
                    }
                }
            } else {
                // Princess at cell i+1
                if(j < value[i+1]){
                    // Can pass through
                    if(dp[i+1][j] < dp[i][j]){
                        dp[i+1][j] = dp[i][j];
                        path[i+1][j] = path[i][j];
                    }
                }
            }
        }
    }
    
    long long maxGold = -1;
    int bestDragons = -1;
    
    for(int j = 0; j < n; j++){
        if(j >= value[n] && dp[n][j] > maxGold){
            maxGold = dp[n][j];
            bestDragons = j;
        }
    }
    
    if(maxGold == -1){
        cout << -1 << "\n";
    } else {
        cout << maxGold << "\n";
        cout << bestDragons << "\n";
        for(int cell : path[n][bestDragons]){
            cout << cell << " ";
        }
        if(bestDragons > 0) cout << "\n";
    }
    
    return 0;
}
