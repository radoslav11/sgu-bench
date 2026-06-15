#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M, S, T;
    cin >> N >> M >> S >> T;
    
    vector<pair<int,int>> edges(M);
    for(int i = 0; i < M; i++){
        cin >> edges[i].first >> edges[i].second;
    }
    
    // BFS from S
    vector<int> dist(N+1, -1);
    queue<int> q;
    dist[S] = 0;
    q.push(S);
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(int i = 0; i < M; i++){
            int a = edges[i].first, b = edges[i].second;
            if(a == u && dist[b] == -1){ dist[b] = dist[u]+1; q.push(b); }
            if(b == u && dist[a] == -1){ dist[a] = dist[u]+1; q.push(a); }
        }
    }
    
    int d = dist[T];
    cout << d << "\n";
    
    for(int level = 0; level < d; level++){
        vector<int> cut;
        for(int i = 0; i < M; i++){
            int a = edges[i].first, b = edges[i].second;
            if((dist[a] == level && dist[b] == level+1) ||
               (dist[b] == level && dist[a] == level+1)){
                cut.push_back(i+1);
            }
        }
        cout << cut.size();
        for(int x : cut) cout << " " << x;
        cout << "\n";
    }
    
    return 0;
}
