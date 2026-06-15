#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, T;
    cin >> N >> T;
    
    vector<int> time(N + 1);
    for(int i = 1; i <= N; i++){
        cin >> time[i];
    }
    
    int M;
    cin >> M;
    
    vector<vector<int>> prereq(N + 1); // prereq[b] = list of problems that must be done before b
    vector<vector<int>> dependents(N + 1); // dependents[a] = list of problems that depend on a
    
    for(int i = 0; i < M; i++){
        int a, b;
        cin >> a >> b;
        prereq[b].push_back(a);
        dependents[a].push_back(b);
    }
    
    int bestCount = 0;
    long long bestPenalty = 0;
    vector<int> bestOrder;
    
    // Try all possible subsets using bitmask
    for(int mask = 0; mask < (1 << N); mask++){
        vector<int> selected;
        long long totalTime = 0;
        
        for(int i = 0; i < N; i++){
            if(mask & (1 << i)){
                selected.push_back(i + 1);
                totalTime += time[i + 1];
            }
        }
        
        if(totalTime > T) continue;
        
        // Check if this subset is valid (all dependencies satisfied)
        bool valid = true;
        for(int prob : selected){
            for(int pre : prereq[prob]){
                if(!(mask & (1 << (pre - 1)))){
                    valid = false;
                    break;
                }
            }
            if(!valid) break;
        }
        
        if(!valid) continue;
        
        // Sort selected problems by time to get optimal order
        sort(selected.begin(), selected.end(), [&](int a, int b){
            return time[a] < time[b];
        });
        
        long long penalty = 0;
        long long elapsed = 0;
        for(int prob : selected){
            elapsed += time[prob];
            penalty += elapsed;
        }
        
        if(selected.size() > bestCount || 
           (selected.size() == bestCount && penalty < bestPenalty)){
            bestCount = selected.size();
            bestPenalty = penalty;
            bestOrder = selected;
        }
    }
    
    cout << bestCount << " " << bestPenalty << "\n";
    for(int i = 0; i < bestOrder.size(); i++){
        if(i > 0) cout << " ";
        cout << bestOrder[i];
    }
    if(bestCount > 0) cout << "\n";
    
    return 0;
}
