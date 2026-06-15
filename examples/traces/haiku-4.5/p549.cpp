#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k;
    cin >> n >> k;
    
    map<int, vector<int>> byMass; // mass -> list of costs (sorted descending)
    
    for(int i = 0; i < n; i++){
        int m, c;
        cin >> m >> c;
        byMass[m].push_back(c);
    }
    
    // Sort costs for each mass in descending order
    for(auto& p : byMass){
        sort(p.second.begin(), p.second.end(), greater<int>());
    }
    
    // Try to find maximum number of sets
    int maxSets = 0;
    long long maxCost = 0;
    
    // Maximum possible sets is limited by n/k
    for(int t = n/k; t >= 1; t--){
        // Check if we can make t sets
        // We need k distinct masses, each with at least t dumbbells
        
        vector<long long> costsPerMass; // cost to include t dumbbells of each viable mass
        
        for(auto& p : byMass){
            if(p.second.size() >= t){
                long long cost = 0;
                for(int i = 0; i < t; i++){
                    cost += p.second[i];
                }
                costsPerMass.push_back(cost);
            }
        }
        
        // If we have at least k masses with t dumbbells each
        if(costsPerMass.size() >= k){
            // Sort in descending order and take top k
            sort(costsPerMass.begin(), costsPerMass.end(), greater<long long>());
            
            long long totalCost = 0;
            for(int i = 0; i < k; i++){
                totalCost += costsPerMass[i];
            }
            
            maxSets = t;
            maxCost = totalCost;
            break; // Since we're going from high to low, first valid t is maximum
        }
    }
    
    cout << maxSets << " " << maxCost << "\n";
    
    return 0;
}
