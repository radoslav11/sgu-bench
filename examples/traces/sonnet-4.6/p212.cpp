#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N,M,L; cin>>N>>M>>L;
    vector<int> lev(N+1);
    for(int i=1;i<=N;i++) cin>>lev[i];
    int src=-1,snk=-1;
    for(int i=1;i<=N;i++){if(lev[i]==1)src=i;if(lev[i]==L)snk=i;}
    struct Edge{int to,cap,flow;};
    vector<Edge> edges;
    vector<vector<int>> g(N+1);
    vector<int> eidx(M);
    for(int i=0;i<M;i++){
        int a,b,c;cin>>a>>b>>c;
        eidx[i]=edges.size();
        g[a].push_back(edges.size());edges.push_back({b,c,0});
        g[b].push_back(edges.size());edges.push_back({a,0,0});
    }
    vector<int> ptr(N+1,0);
    function<long long(int,long long)> dfs=[&](int u,long long pushed)->long long{
        if(u==snk)return pushed;
        for(int&i=ptr[u];i<(int)g[u].size();i++){
            int eid=g[u][i];
            Edge&e=edges[eid];
            if(e.cap-e.flow>0){
                long long d=dfs(e.to,min(pushed,(long long)(e.cap-e.flow)));
                if(d>0){e.flow+=d;edges[eid^1].flow-=d;return d;}
            }
        }
        return 0;
    };
    while(dfs(src,(long long)1e18)>0);
    for(int i=0;i<M;i++)cout<<edges[eidx[i]].flow<<"\n";
}
