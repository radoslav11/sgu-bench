#include <bits/stdc++.h>
using namespace std;

int N, X, Y, M;
vector<pair<int,int>> adj[2001];

void dijkstra(int src, vector<long long>& dist){
    dist.assign(N+1, LLONG_MAX);
    dist[src]=0;
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
    pq.push({0,src});
    while(!pq.empty()){
        auto [d,u]=pq.top(); pq.pop();
        if(d>dist[u]) continue;
        for(auto [v,w]:adj[u]){
            if(d+w<dist[v]){
                dist[v]=d+w;
                pq.push({dist[v],v});
            }
        }
    }
}

int main(){
    scanf("%d %d %d", &N, &X, &Y);
    scanf("%d", &M);
    vector<array<int,3>> edges;
    edges.reserve(M);
    for(int i=0;i<M;i++){
        int x,y,l;
        scanf("%d %d %d", &x,&y,&l);
        adj[x].push_back({y,l});
        adj[y].push_back({x,l});
        edges.push_back({x,y,l});
    }
    vector<long long> dX, dY;
    dijkstra(X, dX);
    dijkstra(Y, dY);
    long long D = dX[Y];
    
    // relevant junctions
    // collect distinct relevant T (= dX[K]) values
    map<long long,int> cntByT; // junction count per T
    for(int k=1;k<=N;k++){
        if(dX[k]!=LLONG_MAX && dY[k]!=LLONG_MAX && dX[k]+dY[k]==D){
            cntByT[dX[k]]++;
        }
    }
    
    vector<long long> sortedT;
    for(auto& p: cntByT) sortedT.push_back(p.first);
    int len = sortedT.size();
    
    vector<int> diff(len+1, 0);
    
    // shortest path edges contribute intervals (a,b) open
    for(auto& e: edges){
        int x=e[0], y=e[1], l=e[2];
        // check dir x->y
        // condition: dX[x]+l+dY[y]==D and dX[y]==dX[x]+l
        long long a=-1,b=-1;
        if(dX[x]!=LLONG_MAX && dY[y]!=LLONG_MAX && dX[y]!=LLONG_MAX
           && dX[x]+l+dY[y]==D && dX[y]==dX[x]+l){
            a=dX[x]; b=dX[y];
        } else if(dX[y]!=LLONG_MAX && dY[x]!=LLONG_MAX && dX[x]!=LLONG_MAX
           && dX[y]+l+dY[x]==D && dX[x]==dX[y]+l){
            a=dX[y]; b=dX[x];
        }
        if(a<0) continue;
        // add +1 to all sortedT with a < T < b
        // first index with sortedT > a
        int lo = upper_bound(sortedT.begin(), sortedT.end(), a) - sortedT.begin();
        // last index with sortedT < b => lower_bound(b)-1
        int hi = (int)(lower_bound(sortedT.begin(), sortedT.end(), b) - sortedT.begin()) - 1;
        if(lo<=hi){
            diff[lo]++;
            diff[hi+1]--;
        }
    }
    
    vector<int> edgeCount(len,0);
    int run=0;
    for(int i=0;i<len;i++){
        run+=diff[i];
        edgeCount[i]=run;
    }
    
    // index lookup for T values
    // build answer
    vector<long long> ans(N+1, 0);
    for(int k=1;k<=N;k++){
        if(dX[k]!=LLONG_MAX && dY[k]!=LLONG_MAX && dX[k]+dY[k]==D){
            long long T=dX[k];
            int idx = lower_bound(sortedT.begin(), sortedT.end(), T) - sortedT.begin();
            ans[k] = cntByT[T] + edgeCount[idx];
        }
    }
    
    // output
    for(int k=1;k<=N;k++){
        printf("%lld", ans[k]);
        if(k<N) printf(" ");
    }
    printf("\n");
    return 0;
}
