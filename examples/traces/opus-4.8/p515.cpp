#include <bits/stdc++.h>
using namespace std;

int n, m;
struct E{int to; int w; int id;};
vector<vector<E>> adj;

vector<long long> dijkstra(int src){
    vector<long long> d(n+1, LLONG_MAX);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    d[src]=0; pq.push({0,src});
    while(!pq.empty()){
        auto [dist,u]=pq.top(); pq.pop();
        if(dist>d[u]) continue;
        for(auto &e: adj[u]){
            long long nd=dist+e.w;
            if(nd<d[e.to]){ d[e.to]=nd; pq.push({nd,e.to}); }
        }
    }
    return d;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    adj.assign(n+1, {});
    for(int i=1;i<=m;i++){
        int a,b,t; cin>>a>>b>>t;
        adj[a].push_back({b,t,i});
        adj[b].push_back({a,t,i});
    }
    int k; cin>>k;
    vector<int> req(k);
    for(int i=0;i<k;i++) cin>>req[i];
    
    if(k==1){
        // single vertex, path with 0 roads
        cout<<0<<"\n"<<"\n";
        return 0;
    }
    
    int r0=req[0];
    vector<long long> d0=dijkstra(r0);
    int s=req[0];
    for(int v:req) if(d0[v]>d0[s]) s=v;
    
    vector<long long> ds=dijkstra(s);
    int t=req[0];
    for(int v:req) if(ds[v]>ds[t]) t=v;
    long long D=ds[t];
    
    // sort required by ds
    sort(req.begin(), req.end(), [&](int x,int y){ return ds[x]<ds[y]; });
    
    // reconstruct segments
    int curVer=0;
    vector<int> ver(n+1,0), ptr(n+1,0);
    vector<int> result;
    
    auto buildSegment=[&](int a,int b){
        curVer++;
        ver[a]=curVer; ptr[a]=0;
        vector<int> nodeStack;
        vector<int> edgeOnPath;
        nodeStack.push_back(a);
        while(!nodeStack.empty()){
            int u=nodeStack.back();
            if(u==b) break;
            bool advanced=false;
            while(ptr[u] < (int)adj[u].size()){
                E &e=adj[u][ptr[u]];
                ptr[u]++;
                int x=e.to;
                if(ds[u]!=LLONG_MAX && ds[x]!=LLONG_MAX && ds[u]+e.w==ds[x] && ds[x]<=ds[b]){
                    if(ver[x]!=curVer){
                        ver[x]=curVer; ptr[x]=0;
                        nodeStack.push_back(x);
                        edgeOnPath.push_back(e.id);
                        advanced=true;
                        break;
                    }
                }
            }
            if(!advanced){
                nodeStack.pop_back();
                if(!edgeOnPath.empty()) edgeOnPath.pop_back();
            }
        }
        for(int id:edgeOnPath) result.push_back(id);
    };
    
    for(int i=0;i+1<(int)req.size();i++){
        buildSegment(req[i], req[i+1]);
    }
    
    cout<<result.size()<<"\n";
    for(size_t i=0;i<result.size();i++){
        cout<<result[i];
        if(i+1<result.size()) cout<<' ';
    }
    cout<<"\n";
    return 0;
}
