#include<bits/stdc++.h>
using crange;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int src, dst;
    cin >> src >> dst;
    
    int N, M;
    cin >> N >> M;
    
    vector<char> initial_color(N+1);
    vector<int> remaining_time(N+1), blue_duration(N+1), purple_duration(N+1);
    
    for(int i = 1; i <= N; i++){
        cin >> initial_color[i] >> remaining_time[i] >> blue_duration[i] >> purple_duration[i];
    }
    
    vector<vector<pair<int,int>>> graph(N+1);
    
    for(int i = 0; i < M; i++){
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }
    
    // Dijkstra with state (time, junction)
    map<pair<int,int>, pair<int,int>> parent; // state -> (prev_junction, prev_time)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    map<pair<int,int>, int> dist;
    
    pq.push({0, src});
    dist[{0, src}] = 0;
    
    auto get_color = [&](int junction, int time) -> char {
        int remaining = remaining_time[junction];
        char color = initial_color[junction];
        int elapsed = time;
        
        while(elapsed > 0){
            int duration = (color == 'B') ? blue_duration[junction] : purple_duration[junction];
            if(elapsed < duration){
                return color;
            }
            elapsed -= duration;
            color = (color == 'B') ? 'P' : 'B';
        }
        return color;
    };
    
    int best_time = -1;
    
    while(!pq.empty()){
        auto [time, u] = pq.top();
        pq.pop();
        
        if(u == dst){
            best_time = time;
            break;
        }
        
        if(dist.count({time, u}) && dist[{time, u}] < time) continue;
        
        char current_color = get_color(u, time);
        
        // Try to move to adjacent junctions
        for(auto [v, edge_time] : graph[u]){
            char next_color = get_color(v, time + edge_time);
            
            if(current_color == next_color){
                int new_time = time + edge_time;
                if(!dist.count({new_time, v}) || dist[{new_time, v}] > new_time){
                    dist[{new_time, v}] = new_time;
                    pq.push({new_time, v});
                    parent[{new_time, v}] = {u, time};
                }
            }
        }
        
        // Try waiting at current junction
        if(time < 100000){
            char wait_color = get_color(u, time + 1);
            if(wait_color != current_color){
                if(!dist.count({time + 1, u}) || dist[{time + 1, u}] > time + 1){
                    dist[{time + 1, u}] = time + 1;
                    pq.push({time + 1, u});
                    parent[{time + 1, u}] = {u, time};
                }
            }
        }
    }
    
    if(best_time == -1){
        cout << 0 << "\n";
    } else {
        cout << best_time << "\n";
        
        vector<int> path;
        int curr = dst;
        int curr_time = best_time;
        
        while(curr != src || curr_time != 0){
            path.push_back(curr);
            if(parent.count({curr_time, curr})){
                auto [prev_u, prev_time] = parent[{curr_time, curr}];
                curr = prev_u;
                curr_time = prev_time;
            } else {
                break;
            }
        }
        path.push_back(src);
        
        reverse(path.begin(), path.end());
        for(int i = 0; i < path.size(); i++){
            if(i > 0) cout << " ";
            cout << path[i];
        }
        cout << "\n";
    }
    
    return 0;
}
