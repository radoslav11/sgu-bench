#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> x(n);
    for(int i = 0; i < n; i++){
        cin >> x[i];
    }
    
    int m, s;
    cin >> m >> s;
    
    vector<vector<long long>> levels(m, vector<long long>(s));
    for(int i = 0; i < m; i++){
        for(int j = 0; j < s; j++){
            cin >> levels[i][j];
        }
    }
    
    // Calculate p such that m = 2^p
    int p = 0;
    int temp = m;
    while(temp > 1){
        temp /= 2;
        p++;
    }
    int mask = (1 << p) - 1;
    
    // DP: dp[i][set] = {min_cost, best_index_chosen}
    vector<vector<pair<long long, int>>> dp(n, vector<pair<long long, int>>(m, {LLONG_MAX, -1}));
    
    // Initialize: first value uses set 0
    for(int k = 0; k < s; k++){
        long long cost = abs(x[0] - levels[0][k]);
        dp[0][0] = min(dp[0][0], {cost, k});
    }
    
    // Fill DP table
    for(int i = 0; i < n - 1; i++){
        for(int cur_set = 0; cur_set < m; cur_set++){
            if(dp[i][cur_set].first == LLONG_MAX) continue;
            
            long long cur_cost = dp[i][cur_set].first;
            
            // Try all levels in current set
            for(int k = 0; k < s; k++){
                long long cost = abs(x[i+1] - levels[cur_set][k]);
                int next_set = k & mask;
                long long total_cost = cur_cost + cost;
                
                if(total_cost < dp[i+1][next_set].first){
                    dp[i+1][next_set] = {total_cost, k};
                }
            }
        }
    }
    
    // Find minimum cost at last position
    long long min_cost = LLONG_MAX;
    int best_final_set = -1;
    for(int set = 0; set < m; set++){
        if(dp[n-1][set].first < min_cost){
            min_cost = dp[n-1][set].first;
            best_final_set = set;
        }
    }
    
    // Reconstruct path
    vector<int> result(n);
    int current_set = best_final_set;
    result[n-1] = dp[n-1][current_set].second;
    
    for(int i = n - 2; i >= 0; i--){
        int chosen_index = result[i+1];
        int prev_set = current_set;
        
        // Find which set and index led to this state
        long long best_prev_cost = LLONG_MAX;
        int best_prev_set = -1;
        int best_prev_k = -1;
        
        for(int set = 0; set < m; set++){
            if(dp[i][set].first == LLONG_MAX) continue;
            for(int k = 0; k < s; k++){
                if((k & mask) == prev_set){
                    long long cost = abs(x[i+1] - levels[set][k]);
                    if(dp[i][set].first + cost == dp[i+1][prev_set].first){
                        best_prev_set = set;
                        best_prev_k = k;
                        break;
                    }
                }
            }
            if(best_prev_k != -1) break;
        }
        
        result[i] = best_prev_k;
        current_set = best_prev_set;
    }
    
    cout << min_cost << "\n";
    for(int i = 0; i < n; i++){
        cout << result[i];
        if(i < n-1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
