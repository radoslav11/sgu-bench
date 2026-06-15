#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, k;
    cin >> n >> k;
    
    // Generate all valid subsets (size <= k)
    vector<set<int>> subsets;
    for(int mask = 0; mask < (1 << n); mask++){
        if(__builtin_popcount(mask) <= k){
            set<int> s;
            for(int i = 0; i < n; i++){
                if(mask & (1 << i)){
                    s.insert(i + 1);
                }
            }
            subsets.push_back(s);
        }
    }
    
    int total = subsets.size();
    
    // BFS to find valid path
    map<set<int>, int> subsetToId;
    for(int i = 0; i < total; i++){
        subsetToId[subsets[i]] = i;
    }
    
    // Try to find Hamiltonian path starting from empty set
    vector<int> path;
    set<int> visited;
    map<set<int>, pair<set<int>, string>> parent;
    
    function<bool(set<int>)> dfs = [&](set<int> curr) -> bool {
        visited.insert(subsetToId[curr]);
        path.push_back(subsetToId[curr]);
        
        if(visited.size() == total && curr.empty()){
            return true;
        }
        
        // Try all valid transitions
        for(auto next : subsets){
            if(visited.count(subsetToId[next])) continue;
            
            // Check if transition is valid
            vector<int> curr_vec(curr.begin(), curr.end());
            vector<int> next_vec(next.begin(), next.end());
            
            // Find difference
            set<int> removed, added;
            for(auto x : curr_vec) if(!next.count(x)) removed.insert(x);
            for(auto x : next_vec) if(!curr.count(x)) added.insert(x);
            
            bool valid = false;
            string op = "";
            
            if(removed.size() == 0 && added.size() == 1){
                // Add one
                valid = true;
                op = "+" + to_string(*added.begin());
            }
            else if(removed.size() == 1 && added.size() == 0){
                // Remove one
                valid = true;
                op = "-" + to_string(*removed.begin());
            }
            else if(removed.size() == 1 && added.size() == 1){
                // Swap adjacent
                int r = *removed.begin();
                int a = *added.begin();
                if(abs(r - a) == 1){
                    valid = true;
                    if(r < a){
                        op = "++" + to_string(r);
                    } else {
                        op = "--" + to_string(a);
                    }
                }
            }
            
            if(valid){
                parent[next] = {curr, op};
                if(dfs(next)) return true;
            }
        }
        
        visited.erase(subsetToId[curr]);
        path.pop_back();
        return false;
    };
    
    set<int> empty_set;
    if(dfs(empty_set)){
        // Reconstruct solution
        string result = "";
        set<int> current;
        for(int i = 1; i < path.size(); i++){
            set<int> next = subsets[path[i]];
            auto [prev, op] = parent[next];
            result += op;
            current = next;
        }
        cout << result << "\n";
    } else {
        cout << "0\n";
    }
    
    return 0;
}
