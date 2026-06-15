#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long N;
    cin >> N;
    
    if(N == 1){
        cout << 0 << "\n";
        return 0;
    }
    
    queue<pair<long long, vector<int>>> q;
    set<long long> visited;
    
    q.push({N, {}});
    visited.insert(N);
    
    while(!q.empty()){
        auto [curr, ops] = q.front();
        q.pop();
        
        if(curr == 1){
            cout << ops.size() << "\n";
            reverse(ops.begin(), ops.end());
            for(int i = 0; i < ops.size(); i++){
                if(i > 0) cout << " ";
                cout << ops[i];
            }
            cout << "\n";
            return 0;
        }
        
        // Try reverse of operation 1: curr = 2X - 1, so X = (curr + 1) / 2
        if((curr + 1) % 2 == 0){
            long long prev = (curr + 1) / 2;
            if(visited.find(prev) == visited.end()){
                visited.insert(prev);
                vector<int> newOps = ops;
                newOps.push_back(1);
                q.push({prev, newOps});
            }
        }
        
        // Try reverse of operation 2: curr = 2X + 1, so X = (curr - 1) / 2
        if((curr - 1) % 2 == 0){
            long long prev = (curr - 1) / 2;
            if(visited.find(prev) == visited.end()){
                visited.insert(prev);
                vector<int> newOps = ops;
                newOps.push_back(2);
                q.push({prev, newOps});
            }
        }
    }
    
    cout << "No solution\n";
    return 0;
}
