#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<tuple<int,int,long long>> edges;
    vector<vector<pair<int,int>>> incoming(N+1), outgoing(N+1);
    
    for(int i = 0; i < M; i++){
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
        incoming[v].push_back({u, i});
        outgoing[u].push_back({v, i});
    }
    
    // BFS to find all reachable states
    map<vector<long long>, int> visited;
    queue<vector<long long>> q;
    set<int> reachable_active;
    
    vector<long long> initial(M);
    for(int i = 0; i < M; i++){
        initial[i] = get<2>(edges[i]);
    }
    
    q.push(initial);
    visited[initial] = 1;
    
    int states_explored = 0;
    while(!q.empty() && states_explored < 100000){
        states_explored++;
        vector<long long> state = q.front();
        q.pop();
        
        // Find active vertices in this state
        vector<int> active;
        for(int v = 1; v <= N; v++){
            bool is_active = true;
            for(auto [u, edge_id] : incoming[v]){
                if(state[edge_id] == 0){
                    is_active = false;
                    break;
                }
            }
            if(is_active && !incoming[v].empty()){
                active.push_back(v);
                reachable_active.insert(v);
            }
        }
        
        // Try firing each active vertex
        for(int v : active){
            vector<long long> new_state = state;
            for(auto [u, edge_id] : incoming[v]){
                new_state[edge_id]--;
            }
            for(auto [w, edge_id] : outgoing[v]){
                new_state[edge_id]++;
            }
            
            if(visited.find(new_state) == visited.end()){
                visited[new_state] = 1;
                q.push(new_state);
            }
        }
    }
    
    for(int v = 1; v <= N; v++){
        if(reachable_active.count(v)){
            cout << "1\n";
        } else {
            cout << "0\n";
        }
    }
    
    return 0;
}
