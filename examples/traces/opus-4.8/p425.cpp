#include <bits/stdc++.h>
using namespace std;

int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    vector<long long> row0(m), cur(m);
    vector<long long> allvals;
    allvals.reserve((size_t)n*m);
    for(int j=0;j<m;j++){ scanf("%lld",&row0[j]); allvals.push_back(row0[j]); }
    vector<pair<long long,long long>> edges;
    for(int i=1;i<n;i++){
        int diffCol=-1;
        for(int j=0;j<m;j++){ scanf("%lld",&cur[j]); allvals.push_back(cur[j]); if(diffCol==-1 && cur[j]!=row0[j]) diffCol=j; }
        edges.push_back({row0[diffCol], cur[diffCol]});
    }
    sort(allvals.begin(), allvals.end());
    allvals.erase(unique(allvals.begin(),allvals.end()), allvals.end());
    unordered_map<long long,int> gid;
    gid.reserve(edges.size()*4+4);
    vector<long long> gval;
    auto getid=[&](long long v)->int{
        auto it=gid.find(v);
        if(it!=gid.end()) return it->second;
        int id=gval.size(); gid[v]=id; gval.push_back(v); return id;
    };
    for(auto &e: edges){ getid(e.first); getid(e.second); }
    int V=gval.size();
    vector<vector<int>> adj(V);
    {
        vector<set<int>> tmp(V);
        for(auto &e: edges){
            int a=gid[e.first], b=gid[e.second];
            tmp[a].insert(b); tmp[b].insert(a);
        }
        for(int v=0;v<V;v++) adj[v].assign(tmp[v].begin(), tmp[v].end());
    }
    vector<int> deg(V), removed(V,0), order;
    order.reserve(V);
    for(int v=0;v<V;v++) deg[v]=adj[v].size();
    for(int iter=0; iter<V; iter++){
        int best=-1;
        for(int v=0;v<V;v++) if(!removed[v]){ if(best==-1||deg[v]<deg[best]) best=v; }
        removed[best]=1; order.push_back(best);
        for(int u: adj[best]) if(!removed[u]) deg[u]--;
    }
    vector<int> color(V,-1);
    for(int idx=V-1; idx>=0; idx--){
        int v=order[idx];
        set<int> used;
        for(int u: adj[v]) if(color[u]!=-1) used.insert(color[u]);
        int c=0; while(used.count(c)) c++;
        color[v]=c;
    }
    printf("Yes\n");
    string out;
    out.reserve(allvals.size()*12);
    char buf[64];
    for(long long v: allvals){
        int c=0;
        auto it=gid.find(v);
        if(it!=gid.end()) c=color[it->second];
        int len=sprintf(buf,"%lld -> %d\n", v, c);
        out.append(buf, len);
    }
    fputs(out.c_str(), stdout);
    return 0;
}
