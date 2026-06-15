#include <bits/stdc++.h>
using namespace std;

int N_trie;
vector<unordered_map<int,int>> children; // trie children, key=edge id
vector<int> endNode; // node where trie node's last edge ends
vector<long long> own; // sum of route weights ending exactly here
vector<int> failLink;
vector<long long> outW;
vector<unordered_map<int,int>> gcache;

int go(int s, int e){
    auto it = children[s].find(e);
    if(it!=children[s].end()) return it->second;
    auto cit = gcache[s].find(e);
    if(cit!=gcache[s].end()) return cit->second;
    int res;
    if(s==0) res=0;
    else res = go(failLink[s], e);
    gcache[s][e]=res;
    return res;
}

int main(){
    int n,m,r,S,T;
    scanf("%d %d %d %d %d",&n,&m,&r,&S,&T);
    vector<int> ea(m+1), eb(m+1); vector<long long> ec(m+1);
    vector<vector<array<long long,3>>> adj(n+1); // to,cost,edgeIdx
    for(int i=1;i<=m;i++){
        int a,b; long long c;
        scanf("%d %d %lld",&a,&b,&c);
        ea[i]=a; eb[i]=b; ec[i]=c;
        adj[a].push_back({(long long)b,c,(long long)i});
    }
    // build trie
    children.push_back({}); endNode.push_back(-1); own.push_back(0);
    for(int q=0;q<r;q++){
        int k; scanf("%d",&k);
        vector<int> es(k);
        long long w=0;
        for(int i=0;i<k;i++){ scanf("%d",&es[i]); w+=ec[es[i]]; }
        int cur=0;
        for(int i=0;i<k;i++){
            int ed=es[i];
            auto it=children[cur].find(ed);
            if(it!=children[cur].end()) cur=it->second;
            else{
                int idx=children.size();
                children.push_back({});
                endNode.push_back(eb[ed]);
                own.push_back(0);
                children[cur][ed]=idx;
                cur=idx;
            }
        }
        own[cur]+=w;
    }
    N_trie=children.size();
    failLink.assign(N_trie,0);
    outW.assign(N_trie,0);
    gcache.assign(N_trie,{});
    // BFS for fail links
    {
        queue<int> bq;
        for(auto &pr: children[0]){
            failLink[pr.second]=0;
            bq.push(pr.second);
        }
        outW[0]=own[0];
        while(!bq.empty()){
            int u=bq.front(); bq.pop();
            outW[u]=own[u]+outW[failLink[u]];
            for(auto &pr: children[u]){
                int e=pr.first, c=pr.second;
                int f=failLink[u];
                while(f!=0 && !children[f].count(e)) f=failLink[f];
                int nf;
                auto it=children[f].find(e);
                if(it!=children[f].end() && it->second!=c) nf=it->second;
                else nf=0;
                failLink[c]=nf;
                bq.push(c);
            }
        }
    }
    // Dijkstra
    // states: 0..N_trie-1 are non-root trie states (1..N_trie-1 used), 0=root used too but root state needs node
    // rootStateId(v) = N_trie + v
    int total = N_trie + n + 1;
    auto rootState=[&](int v){ return N_trie + v; };
    auto nodeOf=[&](int sid)->int{
        if(sid>=N_trie) return sid - N_trie;
        return endNode[sid];
    };
    auto trieOf=[&](int sid)->int{
        if(sid>=N_trie) return 0;
        return sid;
    };
    const long long INF=4e18;
    vector<long long> dist(total, INF);
    vector<int> par(total,-1), parEdge(total,-1);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    int startState = rootState(S);
    dist[startState]=0;
    pq.push({0,startState});
    int ansState=-1;
    while(!pq.empty()){
        auto [d,sid]=pq.top(); pq.pop();
        if(d>dist[sid]) continue;
        int v=nodeOf(sid);
        if(v==T){ ansState=sid; break; }
        int s=trieOf(sid);
        for(auto &ed: adj[v]){
            int w=(int)ed[0]; long long c=ed[1]; int ei=(int)ed[2];
            int sp=go(s,ei);
            long long added=c+outW[sp];
            int nsid;
            if(sp==0) nsid=rootState(w);
            else nsid=sp;
            long long nd=d+added;
            if(nd<dist[nsid]){
                dist[nsid]=nd;
                par[nsid]=sid;
                parEdge[nsid]=ei;
                pq.push({nd,nsid});
            }
        }
    }
    if(ansState==-1){
        printf("-1\n");
        return 0;
    }
    // reconstruct
    vector<int> path;
    int cur=ansState;
    while(par[cur]!=-1){
        path.push_back(parEdge[cur]);
        cur=par[cur];
    }
    reverse(path.begin(),path.end());
    printf("%lld\n", dist[ansState]);
    printf("%d\n",(int)path.size());
    for(size_t i=0;i<path.size();i++){
        printf("%d%c", path[i], i+1<path.size()?' ':'\n');
    }
    if(path.empty()) printf("\n");
    return 0;
}
