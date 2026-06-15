#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int to, cost, id;
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, r, S, T;
    cin >> n >> m >> r >> S >> T;
    S--; T--;
    
    vector<vector<Edge>> graph(n);
    vector<tuple<int,int,int>> edges(m);
    
    for(int i = 0; i < m; i++){
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        edges[i] = {a, b, c};
        graph[a].push_back({b, c, i});
    }
    
    vector<vector<int>> routes(r);
    vector<long long> routeCost(r, 0);
    
    for(int i = 0; i < r; i++){
        int k;
        cin >> k;
        routes[i].resize(k);
        for(int j = 0; j < k; j++){
            cin >> routes[i][j];
            routes[i][j]--;
            routeCost[i] += get<2>(edges[routes[i][j]]);
        }
    }
    
    // State: (cost, node, mask of completed routes, path)
    map<pair<int, long long>, pair<long long, vector<int>>> visited;
    priority_queue<tuple<long long, int, long long, vector<int>>, 
                   vector<tuple<long long, int, long long, vector<int>>>,
                   greater<tuple<long long, int, long long, vector<int>>>> pq;
    
    pq.push({0, S, 0, {}});
    
    while(!pq.empty()){
        auto [cost, node, mask, path] = pq.top();
        pq.pop();
        
        if(node == T){
            cout << cost << "\n";
            cout << path.size() << "\n";
            for(int i = 0; i < path.size(); i++){
                if(i > 0) cout << " ";
                cout << path[i] + 1;
            }
            if(path.size() > 0) cout << "\n";
            return 0;
        }
        
        auto state = make_pair(node, mask);
        if(visited.count(state) && visited[state].first <= cost) continue;
        visited[state] = {cost, path};
        
        for(auto& e : graph[node]){
            vector<int> newPath = path;
            newPath.push_back(e.id);
            long long newCost = cost + e.cost;
            long long newMask = mask;
            
            // Check which routes are completed with this new edge
            for(int i = 0; i < r; i++){
                if(mask & (1LL << i)) continue;
                
                // Check if this route is now complete
                if(routes[i].size() <= newPath.size()){
                    bool match = true;
                    int routeStart = newPath.size() - routes[i].size();
                    for(int j = 0; j < routes[i].size(); j++){
                        if(newPath[routeStart + j] != routes[i][j]){
                            match = false;
                            break;
                        }
                    }
                    if(match){
                        newCost += routeCost[i];
                        newMask |= (1LL << i);
                    }
                }
            }
            
            auto newState = make_pair(e.to, newMask);
            if(!visited.count(newState) || visited[newState].first > newCost){
                pq.push({newCost, e.to, newMask, newPath});
            }
        }
    }
    
    cout << -1 << "\n";
    return 0;
}
