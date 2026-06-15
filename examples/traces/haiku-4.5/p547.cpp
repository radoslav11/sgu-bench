#include<bits/stdc++.h>
using namespace std;

int n, D1, C1, D2, C2;
vector<int> cost;
vector<vector<int>> adj;

bool isConnected(vector<int>& subset) {
    if(subset.empty()) return false;
    set<int> s(subset.begin(), subset.end());
    
    queue<int> q;
    set<int> visited;
    q.push(subset[0]);
    visited.insert(subset[0]);
    
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int v : adj[u]) {
            if(s.count(v) && !visited.count(v)) {
                visited.insert(v);
                q.push(v);
            }
        }
    }
    return visited.size() == subset.size();
}

pair<int, int> getDiameterAndPeripheral(vector<int>& subset) {
    if(subset.size() == 1) return {0, 0};
    
    set<int> s(subset.begin(), subset.end());
    int diameter = 0;
    set<int> peripheral;
    
    for(int start : subset) {
        vector<int> dist(n+1, -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v : adj[u]) {
                if(s.count(v) && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        
        for(int node : subset) {
            if(dist[node] > diameter) {
                diameter = dist[node];
                peripheral.clear();
                peripheral.insert(node);
            } else if(dist[node] == diameter && diameter > 0) {
                peripheral.insert(node);
            }
        }
    }
    
    return {diameter, (int)peripheral.size()};
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> D1 >> C1 >> D2 >> C2;
    cost.resize(n+1);
    adj.resize(n+1);
    
    for(int i = 1; i <= n; i++) cin >> cost[i];
    
    for(int i = 0; i < n-1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    int minCost = INT_MAX;
    vector<int> bestDestroyed;
    
    // Try all possible subsets for King and Queen
    for(int mask1 = 1; mask1 < (1<<n); mask1++) {
        vector<int> king;
        for(int i = 0; i < n; i++) {
            if(mask1 & (1<<i)) king.push_back(i+1);
        }
        
        if(!isConnected(king)) continue;
        auto [d1, c1] = getDiameterAndPeripheral(king);
        if(d1 != D1 || c1 > C1) continue;
        
        for(int mask2 = 1; mask2 < (1<<n); mask2++) {
            if(mask1 & mask2) continue;
            
            vector<int> queen;
            for(int i = 0; i < n; i++) {
                if(mask2 & (1<<i)) queen.push_back(i+1);
            }
            
            if(!isConnected(queen)) continue;
            auto [d2, c2] = getDiameterAndPeripheral(queen);
            if(d2 != D2 || c2 > C2) continue;
            
            // Check no edges between king and queen
            bool adjacent = false;
            set<int> queenSet(queen.begin(), queen.end());
            for(int u : king) {
                for(int v : adj[u]) {
                    if(queenSet.count(v)) {
                        adjacent = true;
                        break;
                    }
                }
                if(adjacent) break;
            }
            if(adjacent) continue;
            
            // Calculate cost
            int totalCost = 0;
            vector<int> destroyed;
            for(int i = 1; i <= n; i++) {
                if(!(mask1 & (1<<(i-1))) && !(mask2 & (1<<(i-1)))) {
                    totalCost += cost[i];
                    destroyed.push_back(i);
                }
            }
            
            if(totalCost < minCost) {
                minCost = totalCost;
                bestDestroyed = destroyed;
            }
        }
    }
    
    if(minCost == INT_MAX) {
        cout << "-1\n";
    } else {
        cout << minCost << "\n";
        for(int i = 0; i < bestDestroyed.size(); i++) {
            if(i > 0) cout << " ";
            cout << bestDestroyed[i];
        }
        if(!bestDestroyed.empty()) cout << "\n";
    }
    
    return 0;
}
