#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<int> a(N);
    for(int i = 0; i < N; i++){
        cin >> a[i];
    }
    
    // dp[product] = vector of lever indices that produce this product
    map<int, vector<int>> dp;
    dp[1] = vector<int>(); // empty set gives product 1
    
    // For each lever
    for(int i = 0; i < N; i++){
        vector<pair<int, vector<int>>> to_add;
        
        // Try adding lever i to each existing product
        for(auto& [prod, levers] : dp){
            int new_prod = (prod * a[i]) % M;
            vector<int> new_levers = levers;
            new_levers.push_back(i + 1);
            to_add.push_back({new_prod, new_levers});
        }
        
        for(auto& [prod, levers] : to_add){
            if(dp.find(prod) == dp.end()){
                dp[prod] = levers;
            }
        }
    }
    
    // Find maximum product
    int max_prod = 0;
    vector<int> best_levers;
    
    for(auto& [prod, levers] : dp){
        if(prod > max_prod){
            max_prod = prod;
            best_levers = levers;
        }
    }
    
    cout << max_prod << "\n";
    sort(best_levers.begin(), best_levers.end());
    for(int i = 0; i < best_levers.size(); i++){
        if(i > 0) cout << " ";
        cout << best_levers[i];
    }
    if(best_levers.size() > 0) cout << "\n";
    
    return 0;
}
