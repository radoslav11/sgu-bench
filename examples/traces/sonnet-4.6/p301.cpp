#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,int> pli;
const ll INF = 1e18;

int main(){
    int N,X,Y; cin>>N>>X>>Y; X--; Y--;
    int M; cin>>M;
    vector<vector<pair<int,int>>> g(N);
    for(int i=0;i<M;i++){
        int u,v,l; cin>>u>>v>>l; u--;v--;
        g[u].push_back({v,l}); g[v].push_back({u,l});
    }
    auto dijkstra=[&](int s){
        vector<ll> d(N,INF); d[s]=0;
        priority_queue<pli,vector<pli>,greater<pli>> pq;
        pq.push({0,s});
        while(!pq.empty()){auto[dd,u]=pq.top();pq.pop();if(dd>d[u])continue;for(auto[v,w]:g[u])if(d[u]+w<d[v]){d[v]=d[u]+w;pq.push({d[v],v});}}
        return d;
    };
    auto dx=dijkstra(X), dy=dijkstra(Y);
    ll dxy=dx[Y];
    vector<int> ans(N,0);
    for(int k=0;k<N;k++){
        if(dx[k]+dy[k]!=dxy){ans[k]=0;continue;}
        ll T=dx[k];
        int cnt=1; // junction k itself
        for(int u=0;u<N;u++){
            if(dx[u]>=T) continue;
            for(auto[v,w]:g[u]){
                if(dx[u]+w==dx[v] && dx[u]+dy[v]==dxy){
                    if(dx[u]<T && T<dx[v]) cnt++;
                }
            }
        }
        ans[k]=cnt;
    }
    for(int i=0;i<N;i++) cout<<ans[i]<<" \n"[i==N-1];
}
