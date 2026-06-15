#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> W(n), L(n);
    vector<int> P(n);
    for(int i = 0; i < n; i++){
        cin >> W[i] >> L[i] >> P[i];
    }
    
    int minCost = INT_MAX;
    vector<int> bestSet;
    
    for(int mask = 1; mask < (1 << n); mask++){
        vector<long long> water(n);
        for(int i = 0; i < n; i++) water[i] = W[i];
        
        int cost = 0;
        for(int i = 0; i < n; i++){
            if(mask & (1 << i)) cost += P[i];
        }
        
        for(int i = 0; i < n; i++){
            if(mask & (1 << i)){
                if(i < n - 1) water[i + 1] += water[i];
            } else {
                if(water[i] > L[i]){
                    mask |= (1 << i);
                    if(i < n - 1) water[i + 1] += water[i];
                }
            }
        }
        
        if((mask & (1 << (n - 1))) && cost < minCost){
            minCost = cost;
            bestSet.clear();
            for(int i = 0; i < n; i++){
                if(mask & (1 << i)) bestSet.push_back(i + 1);
            }
        }
    }
    
    for(int x : bestSet) cout << x << "\n";
    return 0;
}
