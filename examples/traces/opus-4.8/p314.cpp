#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    int s, t;
    scanf("%d %d", &s, &t);
    
    vector<vector<pair<int,int>>> adj(n+1), radj(n+1);
    for(int i=0;i<m;i++){
        int a,b,c;
        scanf("%d %d %d",&a,&b,&c);
        adj[a].push_back({b,c});
        radj[b].push_back({a,c});
    }
    
    const long long INF = LLONG_MAX/4;
    vector<long long> d(n+1, INF);
    // dijkstra on reverse graph from t
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    d[t]=0;
    pq.push({0,t});
    while(!pq.empty()){
        auto [dist,u]=pq.top();pq.pop();
        if(dist>d[u]) continue;
        for(auto&e:radj[u]){
            int v=e.first; long long nd=dist+e.second;
            if(nd<d[v]){
                d[v]=nd;
                pq.push({nd,v});
            }
        }
    }
    
    if(d[s]>=INF){
        for(int i=0;i<k;i++) printf("NO\n");
        return 0;
    }
    
    vector<int> cnt(n+1,0);
    // A* best first search
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> q;
    q.push({d[s], s}); // f = g + d, g=0
    
    int found=0;
    while(!q.empty() && found<k){
        auto [f,u]=q.top();q.pop();
        if(cnt[u]>=k) continue;
        cnt[u]++;
        if(u==t){
            printf("%lld\n", f);
            found++;
            // continue to find more
        }
        if(cnt[u]>k) continue;
        long long g = f - d[u];
        for(auto&e:adj[u]){
            int v=e.first; int c=e.second;
            if(cnt[v]<k){
                long long nf = g + c + d[v];
                q.push({nf, v});
            }
        }
    }
    
    while(found<k){
        printf("NO\n");
        found++;
    }
    
    return 0;
}
