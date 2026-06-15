#include <bits/stdc++.h>
using namespace std;

int N,M;
char initC[305];
int rC[305], tB[305], tP[305];

pair<int,long long> colorAt(int node, long long t){
    int c = (initC[node]=='B')?0:1;
    long long rem = rC[node];
    if(t < rem){
        return {c, rem - t};
    }
    t -= rem;
    c ^= 1;
    while(true){
        long long dur = (c==0)? tB[node] : tP[node];
        if(t < dur) return {c, dur - t};
        t -= dur;
        c ^= 1;
    }
}

int main(){
    int src,dst;
    scanf("%d %d",&src,&dst);
    scanf("%d %d",&N,&M);
    for(int i=1;i<=N;i++){
        char buf[5];
        scanf("%s %d %d %d", buf, &rC[i], &tB[i], &tP[i]);
        initC[i]=buf[0];
    }
    vector<vector<pair<int,int>>> adj(N+1);
    for(int e=0;e<M;e++){
        int i,j,l; scanf("%d %d %d",&i,&j,&l);
        adj[i].push_back({j,l});
        adj[j].push_back({i,l});
    }
    const long long INF = LLONG_MAX/4;
    vector<long long> dist(N+1, INF);
    vector<int> par(N+1,-1);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    dist[src]=0;
    pq.push({0,src});
    while(!pq.empty()){
        auto [d,u]=pq.top(); pq.pop();
        if(d>dist[u]) continue;
        for(auto [v,l]: adj[u]){
            long long t = d;
            bool ok=false;
            int iter=0;
            int maxIter = 4*(100+100)+10;
            while(iter<= maxIter){
                auto [cu,remu]=colorAt(u,t);
                auto [cv,remv]=colorAt(v,t);
                if(cu==cv){ ok=true; break;}
                long long step = min(remu,remv);
                t += step;
                iter++;
            }
            if(!ok) continue;
            long long nd = t + l;
            if(nd < dist[v]){
                dist[v]=nd;
                par[v]=u;
                pq.push({nd,v});
            }
        }
    }
    if(dist[dst]>=INF){
        printf("0\n");
    } else {
        printf("%lld\n", dist[dst]);
        vector<int> path;
        int cur=dst;
        while(cur!=-1){ path.push_back(cur); cur=par[cur];}
        reverse(path.begin(),path.end());
        for(int i=0;i<(int)path.size();i++){
            printf("%d%c", path[i], i+1<(int)path.size()?' ':'\n');
        }
    }
    return 0;
}
