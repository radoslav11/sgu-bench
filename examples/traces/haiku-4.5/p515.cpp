#include<bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

struct Edge {
    int u, v, w;
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<Edge> edges(m);
    vector<vector<pair<int,int>>> adj(n+1);
    
    for(int i = 0; i < m; i++){
        int a, b, t;
        cin >> a >> b >> t;
        edges[i] = {a, b, t};
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }
    
    int k;
    cin >> k;
    vector<int> required(k);
    for(int i = 0; i < k; i++){
        cin >> required[i];
    }
    
    auto dijkstra = [&](int start) -> pair<vector<long long>, vector<pair<int,int>>> {
        vector<long long> d(n+1, INF);
        vector<pair<int,int>> par(n+1, {-1, -1});
        
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        d[start] = 0;
        pq.push({0, start});
        
        while(!pq.empty()){
            auto [cost, u] = pq.top();
            pq.pop();
            
            if(cost > d[u]) continue;
            
            for(auto [v, edgeIdx] : adj[u]){
                long long newCost = cost + edges[edgeIdx].w;
                if(newCost < d[v]){
                    d[v] = newCost;
                    par[v] = {u, edgeIdx};
                    pq.push({newCost, v});
                }
            }
        }
        
        return {d, par};
    };
    
    map<int, pair<vector<long long>, vector<pair<int,int>>>> dists;
    for(int start : required){
        dists[start] = dijkstra(start);
    }
    
    sort(required.begin(), required.end());
    long long bestDist = INF;
    vector<int> bestOrder;
    
    do {
        long long totalDist = 0;
        bool valid = true;
        
        for(int i = 0; i < (int)required.size() - 1; i++){
            if(dists[required[i]].first[required[i+1]] == INF){
                valid = false;
                break;
            }
            totalDist += dists[required[i]].first[required[i+1]];
        }
        
        if(valid && totalDist < bestDist){
            bestDist = totalDist;
            bestOrder = required;
        }
    } while(next_permutation(required.begin(), required.end()));
    
    vector<int> roadSequence;
    for(int i = 0; i < (int)bestOrder.size() - 1; i++){
        int start = bestOrder[i];
        int end = bestOrder[i+1];
        auto [d, par] = dists[start];
        
        vector<int> pathEdges;
        int curr = end;
        while(curr != start){
            auto [p, e] = par[curr];
            pathEdges.push_back(e + 1);
            curr = p;
        }
        
        reverse(pathEdges.begin(), pathEdges.end());
        for(int e : pathEdges){
            roadSequence.push_back(e);
        }
    }
    
    cout << roadSequence.size() << "\n";
    for(int i = 0; i < (int)roadSequence.size(); i++){
        if(i > 0) cout << " ";
        cout << roadSequence[i];
    }
    if(roadSequence.size() > 0) cout << "\n";
    
    return 0;
}
