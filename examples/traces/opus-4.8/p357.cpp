#include <bits/stdc++.h>
using namespace std;

int main(){
    int d[10];
    int up, down, minus;
    cin >> d[1] >> d[2] >> d[3] >> up;
    cin >> d[4] >> d[5] >> d[6] >> down;
    cin >> d[7] >> d[8] >> d[9];
    cin >> minus >> d[0];
    int X, Y;
    cin >> X >> Y;
    
    const int INF = INT_MAX;
    vector<int> dist(100, INF);
    
    dist[X] = 0;
    for(int ch=0; ch<100; ch++){
        int c = INF;
        if(ch <= 9){
            if(d[ch]) c = 1;
        } else {
            int hi = ch/10, lo = ch%10;
            if(minus && d[hi] && d[lo]) c = 3;
        }
        if(c < dist[ch]) dist[ch] = c;
    }
    
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    for(int ch=0; ch<100; ch++){
        if(dist[ch] < INF) pq.push({dist[ch], ch});
    }
    
    while(!pq.empty()){
        auto [du, u] = pq.top(); pq.pop();
        if(du > dist[u]) continue;
        if(up){
            int v = (u+1)%100;
            if(du+1 < dist[v]){ dist[v] = du+1; pq.push({dist[v], v}); }
        }
        if(down){
            int v = (u+99)%100;
            if(du+1 < dist[v]){ dist[v] = du+1; pq.push({dist[v], v}); }
        }
    }
    
    if(dist[Y] == INF) cout << -1 << endl;
    else cout << dist[Y] << endl;
    
    return 0;
}
