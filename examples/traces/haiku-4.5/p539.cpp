#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<long long> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    
    vector<long long> sorted_a = a;
    sort(sorted_a.begin(), sorted_a.end());
    
    vector<vector<pair<int,int>>> steps;
    
    while(a != sorted_a){
        vector<pair<int,int>> current_step;
        vector<bool> used(n, false);
        
        for(int i = 0; i < n; i++){
            if(used[i] || a[i] == sorted_a[i]) continue;
            
            // Find a position j where a[j] should be at position i
            for(int j = i + 1; j < n; j++){
                if(used[j]) continue;
                
                // Check if swapping i and j helps both elements
                // Element at i should go to position of sorted_a[i]
                // Element at j should go to position of sorted_a[j]
                // After swap: a[i] = old a[j], a[j] = old a[i]
                
                if(a[j] == sorted_a[i] && a[i] == sorted_a[j]){
                    // Both would be in correct position
                    current_step.push_back({i, j});
                    used[i] = true;
                    used[j] = true;
                    swap(a[i], a[j]);
                    break;
                }
                else if(a[j] == sorted_a[i]){
                    // At least a[j] moves to correct position
                    current_step.push_back({i, j});
                    used[i] = true;
                    used[j] = true;
                    swap(a[i], a[j]);
                    break;
                }
            }
        }
        
        if(!current_step.empty()){
            steps.push_back(current_step);
        } else {
            // Fallback: if no optimal swap found, just swap with first incorrect
            for(int i = 0; i < n; i++){
                if(a[i] != sorted_a[i]){
                    for(int j = i + 1; j < n; j++){
                        if(a[j] != sorted_a[j]){
                            vector<pair<int,int>> step;
                            step.push_back({i, j});
                            steps.push_back(step);
                            swap(a[i], a[j]);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    
    cout << steps.size() << "\n";
    for(auto& step : steps){
        cout << step.size();
        for(auto& p : step){
            cout << " " << (p.first + 1) << " " << (p.second + 1);
        }
        cout << "\n";
    }
    
    return 0;
}
