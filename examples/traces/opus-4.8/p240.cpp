#include <bits/stdc++.h>
using namespace std;

int N,M,H,S,E;
struct Edge{int to,T,R,P;};
vector<Edge> adj[105];
bool isExit[105];

// returns earliest arrival; also fills parent
const long long INF = LLONG_MAX;

bool feasible(int X, vector<long long>&dist, vector<int>&par){
    dist.assign(N+1, INF);
    par.assign(N+1, -1);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    dist[S]=0;
    pq.push({0,S});
    while(!pq.empty()){
        auto [d,u]=pq.top(); pq.pop();
        if(d>dist[u]) continue;
        for(auto&e: adj[u]){
            long long finishTemp = (long long)e.R + (long long)e.P*(d + e.T);
            if(finishTemp <= X){
                long long nd = d + e.T;
                if(nd < dist[e.to]){
                    dist[e.to]=nd;
                    par[e.to]=u;
                    pq.push({nd,e.to});
                }
            }
        }
    }
    for(int i=1;i<=N;i++) if(isExit[i] && dist[i]<INF) return true;
    return false;
}
