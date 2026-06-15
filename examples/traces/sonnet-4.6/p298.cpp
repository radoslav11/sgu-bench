#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M;
    cin>>N>>M;
    vector<tuple<int,int,int>> edges;
    for(int i=0;i<M;i++){
        int x,y,c; cin>>x>>y>>c;
        // A[x] >= A[y] + c => edge y->x weight c
        edges.push_back({y,x,c});
    }
    const int INF=1e9;
    vector<int> dist(N+1,INF);
    dist[1]=0;
    for(int iter=0;iter<N;iter++){
        for(auto&[u,v,w]:edges){
            if(dist[u]!=INF && dist[u]+w>dist[v]) dist[v]=dist[u]+w;
        }
    }
    for(auto&[u,v,w]:edges){
        if(dist[u]!=INF && dist[u]+w>dist[v]){cout<<-1;return 0;}
    }
    int mn=0,mx=0;
    for(int i=1;i<=N;i++) if(dist[i]!=INF){mn=min(mn,dist[i]);mx=max(mx,dist[i]);}
    int omin=-10000-mn, omax=10000-mx;
    if(omin>omax){cout<<-1;return 0;}
    int offset=omin;
    for(int i=1;i<=N;i++){
        if(dist[i]==INF) cout<<-10000;
        else cout<<dist[i]+offset;
        if(i<N)cout<<" ";
    }
}
