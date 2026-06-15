#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e18;

int N, M, R;
vector<int> prob;
vector<tuple<int,int,int>> edges;
vector<vector<pair<int,int>>> adj;

bool isConnected() {
    vector<bool> visited(N, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    int count = 1;
    
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(auto [v, w] : adj[u]) {
            if(!visited[v]) {
                visited[v] = true;
                count++;
                q.push(v);
            }
        }
    }
    return count == N;
}

vector<double> dijkstra(int source, double posOnEdge = -1, int edgeIdx = -1) {
    vector<double> dist(N, INF);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
    
    if(posOnEdge < 0) {
        dist[source] = 0;
        pq.push({0, source});
    } else {
        auto [a, b, len] = edges[edgeIdx];
        dist[a] = posOnEdge;
        dist[b] = len - posOnEdge;
        pq.push({posOnEdge, a});
        pq.push({len - posOnEdge, b});
    }
    
    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if(d > dist[u] + EPS) continue;
        
        for(auto [v, w] : adj[u]) {
            if(dist[u] + w < dist[v] - EPS) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M >> R;
    prob.resize(N);
    adj.resize(N);
    
    for(int i = 0; i < N; i++) {
        cin >> prob[i];
    }
    
    for(int i = 0; i < M; i++) {
        int a, b, l;
        cin >> a >> b >> l;
        a--; b--;
        edges.push_back({a, b, l});
        adj[a].push_back({b, l});
        adj[b].push_back({a, l});
    }
    
    if(!isConnected()) {
        cout << -1 << endl;
        return 0;
    }
    
    double minExpected = INF;
    
    // Try each city
    for(int i = 0; i < N; i++) {
        auto dist = dijkstra(i);
        bool valid = true;
        
        double expected = 0;
        for(int j = 0; j < N; j++) {
            expected += prob[j] * dist[j];
        }
        expected /= 10000.0;
        minExpected = min(minExpected, expected);
    }
    
    // Try positions on edges
    for(int e = 0; e < M; e++) {
        auto [a, b, len] = edges[e];
        
        // Position must be at distance >= R from both endpoints
        // and at distance >= R means the position is at distance pos from a
        // where pos >= R and len - pos >= R
        // So R <= pos <= len - R
        
        double minPos = R;
        double maxPos = len - R;
        
        if(minPos > maxPos - EPS) continue; // no valid position
        
        // Check minPos
        {
            auto dist = dijkstra(a, minPos, e);
            bool valid = true;
            double expected = 0;
            for(int j = 0; j < N; j++) {
                expected += prob[j] * dist[j];
            }
            expected /= 10000.0;
            minExpected = min(minExpected, expected);
        }
        
        // Check maxPos
        {
            auto dist = dijkstra(a, maxPos, e);
            bool valid = true;
            double expected = 0;
            for(int j = 0; j < N; j++) {
                expected += prob[j] * dist[j];
            }
            expected /= 10000.0;
            minExpected = min(minExpected, expected);
        }
    }
    
    if(minExpected >= INF - 1) {
        cout << -1 << endl;
    } else {
        cout << fixed << setprecision(5) << minExpected << endl;
    }
    
    return 0;
}
