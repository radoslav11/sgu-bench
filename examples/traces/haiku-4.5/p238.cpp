#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    vector<vector<int>> initial(n);
    for(int i = 0; i < n; i++){
        int c;
        cin >> c;
        for(int j = 0; j < c; j++){
            int bag;
            cin >> bag;
            bag--; // 0-indexed
            initial[i].push_back(bag);
        }
    }
    
    // State: for each bag, which bags are directly inside (as bitmask)
    // We need to track which bags are on the floor too
    // A bag is on floor if it's not inside any other bag
    
    map<vector<long long>, int> visited;
    queue<vector<long long>> q;
    
    // Convert initial state to bitmask representation
    auto toBitmask = [&](const vector<vector<int>>& state) -> vector<long long> {
        vector<long long> result(n, 0);
        for(int i = 0; i < n; i++){
            for(int bag : state[i]){
                result[i] |= (1LL << bag);
            }
        }
        return result;
    };
    
    auto toState = [&](const vector<long long>& masks) -> vector<vector<int>> {
        vector<vector<int>> result(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(masks[i] & (1LL << j)){
                    result[i].push_back(j);
                }
            }
        }
        return result;
    };
    
    vector<long long> initial_state = toBitmask(initial);
    q.push(initial_state);
    visited[initial_state] = 1;
    
    while(!q.empty()){
        auto current = q.front();
        q.pop();
        
        auto state = toState(current);
        
        // Try opening each bag on the floor
        vector<bool> on_floor(n, true);
        for(int i = 0; i < n; i++){
            for(int j : state[i]){
                on_floor[j] = false;
            }
        }
        
        for(int i = 0; i < n; i++){
            if(!on_floor[i]) continue;
            
            // Open bag i
            vector<vector<int>> next_state = state;
            long long inside = current[i];
            
            // Calculate complement: all bags not inside bag i
            long long complement = 0;
            for(int j = 0; j < n; j++){
                if(!(inside & (1LL << j)) && j != i){
                    complement |= (1LL << j);
                }
            }
            
            next_state[i].clear();
            for(int j = 0; j < n; j++){
                if(complement & (1LL << j)){
                    next_state[i].push_back(j);
                }
            }
            
            auto next_mask = toBitmask(next_state);
            if(visited.find(next_mask) == visited.end()){
                visited[next_mask] = 1;
                q.push(next_mask);
            }
        }
    }
    
    cout << visited.size() << endl;
    
    return 0;
}
