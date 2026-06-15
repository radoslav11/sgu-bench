#include<bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, X, Y;
    cin >> N >> X >> Y;
    X--; Y--; // 0-indexed
    
    int M;
    cin >> M;
    
    vector<vector<pair<int,long long>>> adj(N);
    
    for(int i = 0; i < M; i++){
        int x, y;
        long long l;
        cin >> x >> y >> l;
        x--; y--;
        adj[x].push_back({y, l});
        adj[y].push_back({x, l});
    }
    
    // Dijkstra from X
    vector<long long> distX(N, INF);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
    distX[X] = 0;
    pq.push({0, X});
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        if(d > distX[u]) continue;
        for(auto [v, w] : adj[u]){
            if(distX[u] + w < distX[v]){
                distX[v] = distX[u] + w;
                pq.push({distX[v], v});
            }
        }
    }
    
    // Dijkstra from Y
    vector<long long> distY(N, INF);
    distY[Y] = 0;
    pq.push({0, Y});
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        if(d > distY[u]) continue;
        for(auto [v, w] : adj[u]){
            if(distY[u] + w < distY[v]){
                distY[v] = distY[u] + w;
                pq.push({distY[v], v});
            }
        }
    }
    
    long long shortest = distX[Y];
    
    vector<long long> answer(N, 0);
    
    for(int k = 0; k < N; k++){
        // Check if junction k is on a shortest path
        if(distX[k] + distY[k] == shortest){
            answer[k]++; // The junction itself
            
            // Count points on roads from k
            for(auto [j, len] : adj[k]){
                // Road from k to j is on shortest path if:
                // distX[k] + len + distY[j] == shortest OR
                // distX[j] + len + distY[k] == shortest
                if(distX[k] + len + distY[j] == shortest){
                    answer[k] += len - 1; // Points on the road (excluding endpoints)
                }
                else if(distX[j] + len + distY[k] == shortest){
                    answer[k] += len - 1;
                }
            }
        }
    }
    
    for(int i = 0; i < N; i++){
        cout << answer[i];
        if(i < N-1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
