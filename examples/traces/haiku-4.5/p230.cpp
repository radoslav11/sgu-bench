#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> adj(N + 1);
    vector<int> indegree(N + 1, 0);
    
    for(int i = 0; i < M; i++){
        int P, Q;
        cin >> P >> Q;
        // P is lighter than Q, so P comes before Q in topological order
        adj[P].push_back(Q);
        indegree[Q]++;
    }
    
    // Topological sort using Kahn's algorithm
    queue<int> q;
    for(int i = 1; i <= N; i++){
        if(indegree[i] == 0){
            q.push(i);
        }
    }
    
    vector<int> topo_order;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        for(int v : adj[u]){
            indegree[v]--;
            if(indegree[v] == 0){
                q.push(v);
            }
        }
    }
    
    // Check if there's a cycle
    if(topo_order.size() != N){
        cout << "No solution\n";
        return 0;
    }
    
    // Assign coins to boxes based on topological order
    vector<int> result(N + 1);
    for(int i = 0; i < N; i++){
        result[topo_order[i]] = i + 1;
    }
    
    // Output
    for(int i = 1; i <= N; i++){
        if(i > 1) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
