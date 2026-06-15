#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,int> pli;
int N,M,H,S,E_cnt;
struct Edge{int to,T;ll R,P;};
vector<Edge> adj[105];
bool isExit[105];
vector<ll> dijkstra(ll thr,vector<int>&par){
    vector<ll> dist(N+1,LLONG_MAX);par.assign(N+1,-1);
    priority_queue<pli,vector<pli>,greater<pli>> pq;
    dist[S]=0;pq.push({0,S});
    while(!pq.empty()){auto[d,u]=pq.top();pq.pop();if(d>dist[u])continue;for(auto&e:adj[u]){ll temp=e.R+e.P*(d+e.T);if(temp<=thr){ll nd=d+e.T;if(nd<dist[e.to]){dist[e.to]=nd;par[e.to]=u;pq.push({nd,e.to});}}}}
    return dist;
}
int main(){
    cin>>N>>M>>H>>S>>E_cnt;
    for(int i=0;i<M;i++){int a,b,t;ll r,p;cin>>a>>b>>t>>r>>p;adj[a].push_back({b,t,r,p});adj[b].push_back({a,t,r,p});}
    for(int i=0;i<E_cnt;i++){int f;cin>>f;isExit[f]=true;}
    ll lo=0,hi=(ll)2e13,ans=-1;
    vector<int> bestPar;
    int bestExit=-1;
    while(lo<=hi){ll mid=lo+(hi-lo)/2;vector<int>par;auto dist=dijkstra(mid,par);bool ok=false;for(int i=1;i<=N;i++)if(isExit[i]&&dist[i]!=LLONG_MAX){ok=true;bestExit=i;break;}if(ok){ans=mid;hi=mid-1;bestPar=par;}else lo=mid+1;}
    if(ans==-1){cout<<"NO\n";}else{// rerun to get correct parents at ans
        vector<int>par;dijkstra(ans,par);
        auto dist2=dijkstra(ans,par);
        bestExit=-1;for(int i=1;i<=N;i++)if(isExit[i]&&dist2[i]!=LLONG_MAX){bestExit=i;break;}
        cout<<"YES\n"<<ans<<"\n";
        vector<int>path;int cur=bestExit;
        while(cur!=S){path.push_back(cur);cur=par[cur];}
        path.push_back(S);
        reverse(path.begin(),path.end());
        cout<<path.size();for(int x:path)cout<<" "<<x;cout<<"\n";}
}
