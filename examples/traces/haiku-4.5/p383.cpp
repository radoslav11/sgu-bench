#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<double, double>> oases(n);
    for(int i = 0; i < n; i++){
        cin >> oases[i].first >> oases[i].second;
    }
    
    // Precompute distances between all pairs
    vector<vector<double>> dist(n, vector<double>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            double dx = oases[i].first - oases[j].first;
            double dy = oases[i].second - oases[j].second;
            dist[i][j] = sqrt(dx*dx + dy*dy);
        }
    }
    
    int q;
    cin >> q;
    
    while(q--){
        int s, t;
        cin >> s >> t;
        s--; t--;
        
        // Modified Dijkstra - minimize maximum edge on path
        vector<double> maxDist(n, 1e18);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        
        maxDist[s] = 0;
        pq.push({0, s});
        
        while(!pq.empty()){
            auto [currMax, u] = pq.top();
            pq.pop();
            
            if(currMax > maxDist[u]) continue;
            
            for(int v = 0; v < n; v++){
                if(u == v) continue;
                // The maximum on path through v is max(currMax, dist[u][v])
                double newMax = max(currMax, dist[u][v]);
                if(newMax < maxDist[v]){
                    maxDist[v] = newMax;
                    pq.push({newMax, v});
                }
            }
        }
        
        cout << fixed << setprecision(10) << maxDist[t] << "\n";
    }
    
    return 0;
}
